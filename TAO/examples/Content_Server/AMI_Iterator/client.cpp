// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/Process_Manager.h"
#include "ace/OS_NS_unistd.h"
#include "orbsvcs/CosNamingC.h"
#include "Web_ServerC.h"
#include "Iterator_Handler.h"

ACE_RCSID (AMI_Iterator,
           client,
           "$Id$")

// Obtain reference to Iterator_Factory
Web_Server::Iterator_Factory_ptr
get_iterator (CORBA::ORB_ptr orb
              ACE_ENV_ARG_DECL);

// Perform file requests
void invoke_requests (int argc,
                      char *argv[],
                      int *request_count,
                      Web_Server::Iterator_Factory_ptr f
                      ACE_ENV_ARG_DECL);

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (argc < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: client filename ")
                           ACE_TEXT ("[filename ...]\n")),
                          -1);
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Mighty ORB"
                                            ACE_ENV_ARG_PARAMETER);

      // Get the Root POA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);

      // Activate the POA manager.
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      // Get an Iterator_Factory reference.
      Web_Server::Iterator_Factory_var factory =
        ::get_iterator (orb.in ()
                        ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (factory.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Object pointed to by:\n  %s\n")
                           ACE_TEXT ("is not an Iterator_Factory object.\n"),
                           argv[1]),
                          -1);

      // 1 millisecond delay to reduce "busy waiting" in ORB event
      // loop.  (simulating "work")
      ACE_Time_Value tv (0, 1000);

      // Variable used to keep track of when file retrieval has
      // completed.
      int request_count = 0;

      ::invoke_requests (argc,
                         argv,
                         &request_count,
                         factory.in ()
                         ACE_ENV_ARG_PARAMETER);

      // Run the ORB event loop.
      while (request_count > 0)
        {
          CORBA::Boolean more_work;

          more_work = orb->work_pending ();

          if (more_work)
            {
              orb->perform_work ();
            }
          else
            ACE_OS::sleep (tv);
        }

      orb->shutdown (0 ACE_ENV_ARG_PARAMETER);

      orb->destroy ();
    }
  ACE_CATCH (Web_Server::Error_Result, exc)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Caught Web Server exception with ")
                         ACE_TEXT ("status %d\n"),
                         exc.status),
                        -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));

      return -1;
    }
  ACE_ENDTRY;

  // Wait for all children to exit.
  ACE_Process_Manager::instance ()->wait ();

  return 0;
}

Web_Server::Iterator_Factory_ptr
get_iterator (CORBA::ORB_ptr o
              ACE_ENV_ARG_DECL)
{
  CORBA::ORB_var orb = CORBA::ORB::_duplicate (o);

  // Get a reference to the Name Service.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("NameService"
                                     ACE_ENV_ARG_PARAMETER);

  // Narrow to a Naming Context
  CosNaming::NamingContext_var nc =
    CosNaming::NamingContext::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Nil reference to Name Service\n")));
      return Web_Server::Iterator_Factory::_nil ();
    }

  // Create a name.
  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup ("Iterator_Factory");
  name[0].kind = CORBA::string_dup ("");

  obj = nc->resolve (name ACE_ENV_ARG_PARAMETER);

  Web_Server::Iterator_Factory_ptr factory =
    Web_Server::Iterator_Factory::_narrow (obj.in ());

  return factory;
}

void invoke_requests (int argc,
                      char *argv[],
                      int *request_count,
                      Web_Server::Iterator_Factory_ptr f
                      ACE_ENV_ARG_DECL)
{
  Web_Server::Iterator_Factory_var factory =
    Web_Server::Iterator_Factory::_duplicate (f);

  // Activate and run the reply handlers.
  for (int i = 0;
       i < argc - 1;  // Don't include the program name.
       ++i)
    {
      Iterator_Handler *handler = 0;
      ACE_NEW_THROW_EX (handler,
                        Iterator_Handler,
                        CORBA::NO_MEMORY ());

      // Transfer ownership to the POA.
      PortableServer::ServantBase_var tmp (handler);

      // This ends up being an AMI call, so it won't block.
      handler->run (request_count,
                    argv[i + 1],
                    factory.in ()
                    ACE_ENV_ARG_PARAMETER);
    }
}
