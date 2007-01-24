// $Id$

// ==========================================================================
// = LIBRARY
//    TAO/example/POA/POA_BiDir
//
// = FILENAME
//    POA_BiDir.cpp
//
// = DESCRIPTION
//    This program is an extension of $TAO_ROOT/example/POA/NewPOA. In
//    the many POA's created this example adds a BiDirGIOP policy to
//    one of the POA's.
//
// = AUTHOR
//    Balachandran Natarajan <bala@cs.wustl.edu>
// ========================================================================

#include "tao/PortableServer/PortableServer.h"

#include "tao/BiDir_GIOP/BiDirGIOP.h"

#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB.h"

#include "ace/SString.h"


ACE_RCSID (POA_BiDir,
           POA_BiDir,
           "$Id$")

void
print_poa (PortableServer::POA_ptr poa
           ACE_ENV_ARG_DECL)
{
  CORBA::String_var poa_name =
    poa->the_name ();

  CORBA::OctetSeq_var poa_id =
    poa->id ();

  ACE_DEBUG ((LM_DEBUG,
              "POA name = %s\n",
              poa_name.in ()));

  ACE_DEBUG ((LM_DEBUG,
              "POA id = "));

  for (CORBA::ULong i = 0;
       i != poa_id->length ();
       ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%o",
                  poa_id[i]));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"));

  PortableServer::POAList_var children =
    poa->the_children ();

  for (CORBA::ULong index = 0;
       index != children->length ();
       ++index)
    {
      print_poa (children[index]
                 ACE_ENV_ARG_PARAMETER);
    }
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // The first step Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            ACE_ENV_ARG_PARAMETER);

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      // _narrow() the Object to get the POA object, i.e., the root_poa.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);

      // Policies for the new POAs
      CORBA::PolicyList policies (2);
      policies.length (2);

      // Threading policy
      policies[0] =
        root_poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL ACE_ENV_ARG_PARAMETER);

      // Lifespan policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::TRANSIENT ACE_ENV_ARG_PARAMETER);

      // Creation of the firstPOA
      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->create_POA (name.c_str (),
                              PortableServer::POAManager::_nil (),
                              policies
                              ACE_ENV_ARG_PARAMETER);

      // Creation of the new POA, i.e. firstPOA/secondPOA
      name = "secondPOA";
      PortableServer::POA_var second_poa =
        first_poa->create_POA (name.c_str (),
                               PortableServer::POAManager::_nil (),
                               policies
                               ACE_ENV_ARG_PARAMETER);

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      CORBA::PolicyList bidir_policy (1);
      bidir_policy.length (1);

      bidir_policy[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            pol
                            ACE_ENV_ARG_PARAMETER);


      // Creating thirdPOA.
      name = "thirdPOA";

      PortableServer::POA_var third_poa =
        root_poa->create_POA (name.c_str (),
                              PortableServer::POAManager::_nil (),
                              bidir_policy
                              ACE_ENV_ARG_PARAMETER);

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong k = 0;
           k < bidir_policy.length ();
           ++k)
        {
          bidir_policy[k]->destroy ();
        }

      // Get the names of all the POAs and print them out.

      CORBA::String_var root_poa_name =
        root_poa->the_name ();

      CORBA::String_var first_poa_name =
        first_poa->the_name ();

      CORBA::String_var second_poa_name =
        second_poa->the_name ();

      CORBA::String_var third_poa_name =
        third_poa->the_name ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n%s\n%s\n%s\n",
                  root_poa_name.in (),
                  first_poa_name.in (),
                  second_poa_name.in (),
                  third_poa_name.in ()));

      print_poa (root_poa.in ()
                 ACE_ENV_ARG_PARAMETER);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught in main ");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
