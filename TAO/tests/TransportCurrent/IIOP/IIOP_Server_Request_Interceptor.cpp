// -*- C++ -*-

#include "ace/Log_Msg.h"
#include "tao/ORB_Constants.h"
#include "tao/TransportCurrent/TC_IIOPC.h"

ACE_RCSID (ForwardRequest,
           Server_Request_Interceptor,
           "$Id$")

#include "IIOP_Server_Request_Interceptor.h"

namespace Test {

  using namespace TAO;

  IIOP_Server_Request_Interceptor::IIOP_Server_Request_Interceptor (const char* orbid, TEST test)
    : Server_Request_Interceptor (orbid, test)
    , iiop_test_successful_ (true)
    , mult_thr_test_successful_ (true)
  {
  }

  IIOP_Server_Request_Interceptor::~IIOP_Server_Request_Interceptor (void)
  {
  }

  char *
  IIOP_Server_Request_Interceptor::name (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup ("II SRI");
  }

  ///
  TAO::Transport::IIOP::Current_ptr
  IIOP_Server_Request_Interceptor::resolve_iiop_transport_current (const char* orbid
                                                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CORBA::String_var name (this->name ());

    int tmpargc = 0;
    CORBA::ORB_var orb = CORBA::ORB_init (tmpargc,
                                          0,
                                          orbid
                                          ACE_ENV_ARG_PARAMETER);

    CORBA::Object_var tcobject =
      orb->resolve_initial_references (ACE_TEXT_ALWAYS_CHAR ("TAO::Transport::IIOP::Current")
                                       ACE_ENV_ARG_PARAMETER);

    return TAO::Transport::IIOP::Current::_narrow (tcobject.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  }

  /// On every request, a client-supplied (via the context) id is used
  /// as index in an array, where we store the endpoint
  void
  IIOP_Server_Request_Interceptor::push_request_info (size_t requestID
                                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CORBA::String_var name (this->name ());

    TAO::Transport::IIOP::Current_var tc =
      resolve_iiop_transport_current (this->orb_id_.in ()
                                      ACE_ENV_ARG_PARAMETER);

    CORBA::String_var host (tc->remote_host());
    EndPoint ep (tc->remote_port(), host.in ());

    if (requestID < sizeof (this->endPoints_) / sizeof (*this->endPoints_))
      {
        endPoints_[requestID] = ep;

        if (TAO_debug_level > 1)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%s (%P|%t) ")
                      ACE_TEXT ("push_request_info: %d ...\n"),
                      name.in (),
                      requestID));
      }
    else
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%s (%P|%t) ")
                  ACE_TEXT ("push_request_info: Can't track that many requests %d\n"),
                  name.in (),
                  requestID));

  }

  void
  IIOP_Server_Request_Interceptor::pop_request_info (size_t requestID)
  {
    static EndPoint dummy;
    CORBA::String_var name (this->name ());

    if (TAO_debug_level > 1)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s (%P|%t) ")
                  ACE_TEXT ("pop_request_info: %d ...\n"),
                  name.in (),
                  requestID));

    if (requestID >= sizeof (this->endPoints_) / sizeof (*this->endPoints_))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%s (%P|%t) ")
                    ACE_TEXT ("pop_request_info: Can't track that many requests %d\n"),
                    name.in (),
                    requestID));
        return;
      }


    ACE_TRY
      {
        TAO::Transport::IIOP::Current_var tc =
          resolve_iiop_transport_current (this->orb_id_.in ()
                                          ACE_ENV_ARG_PARAMETER);

        CORBA::String_var host (tc->remote_host());
        EndPoint ep (tc->remote_port(), host.in ());

        if (ep != endPoints_[requestID])
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%s (%P|%t) ")
                        ACE_TEXT ("pop_request_info: The expected host and port don't match for request %d\n"),
                        name.in (),
                        requestID));
            return;
          }
      }
    ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
      {
        // Last reply after the orb has been shut down. Calling
        // resolve_iiop_transport_current in this case will cause
        // BAD_INV_ORDER, so instead we swallow the exception and bid
        // goodbye.
      }
    ACE_ENDTRY;

    endPoints_[requestID] = dummy;
  }


  /// Checks if all the endpoints, encountered on the way in have been
  /// cleaned on the way out of the interception point
  bool
  IIOP_Server_Request_Interceptor::self_test (void)
  {
    CORBA::String_var name (this->name ());
    bool has_remaining_endpoints = false;
    for (size_t count = 0;
         count < (sizeof (this->endPoints_) / sizeof (*this->endPoints_));
         count ++)
      {
        if (endPoints_[count].port_ != 0)
          {
            has_remaining_endpoints = true;
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%s (%P|%t) Endpoint at ")
                        ACE_TEXT ("index=%d has not been removed yet\n"),
                        name.in (),
                        count));
          }
      }

    return Server_Request_Interceptor::self_test ()
      && iiop_test_successful_
      && !has_remaining_endpoints;
  }



  void
  IIOP_Server_Request_Interceptor::inbound_process_context  (PortableInterceptor::ServerRequestInfo_ptr ri
                                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CORBA::String_var name (this->name ());
    CORBA::String_var op (ri->operation(ACE_ENV_ARG_PARAMETER));

    if (TAO_debug_level >=1)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("%s (%P|%t) Intercepted operation %s ()\n"),
                  name.in (),
                  op.in ()));

    ACE_TRY
      {
        IOP::ServiceId id = Test::Transport::CurrentTest::ContextTag;
        IOP::ServiceContext_var sc =
          ri->get_request_service_context (id
                                           ACE_ENV_ARG_PARAMETER);

        const char *buf =
          reinterpret_cast <const char *> (sc->context_data.get_buffer ());

        long requestID = ACE_OS::atoi (buf);

        this->push_request_info (requestID);
      }
    ACE_CATCHANY
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s (%P|%t) Service context")
                    ACE_TEXT (" is unavailable when invoking %s (). ")
                    ACE_TEXT ("A colocated invocation would have ")
                    ACE_TEXT ("no service context.\n"),
                    name.in (),
                    op.in ()));
      }
    ACE_ENDTRY;

  }

  void
  IIOP_Server_Request_Interceptor::outbound_process_context (PortableInterceptor::ServerRequestInfo_ptr ri
                                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    IOP::ServiceId id = Test::Transport::CurrentTest::ContextTag;

    IOP::ServiceContext_var sc =
      ri->get_request_service_context (id ACE_ENV_ARG_PARAMETER);

    const char *buf =
      reinterpret_cast <const char *> (sc->context_data.get_buffer ());

    this->pop_request_info (ACE_OS::atoi (buf));
  }






  void
  IIOP_Server_Request_Interceptor::receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr ri
                                                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    ACE_TRY
      {
        inbound_process_context (ri ACE_ENV_ARG_PARAMETER);
      }
    ACE_CATCHANY
      {
        CORBA::String_var name (this->name ());
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("%s (%P|%t) Inbound_process_context failed in  ")
                    ACE_TEXT(" receive_request_service_contexts.\n"),
                    name.in ()));
      }
    ACE_ENDTRY;

    Server_Request_Interceptor::receive_request_service_contexts (ri ACE_ENV_ARG_PARAMETER);

  }


  void
  IIOP_Server_Request_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_TRY
      {
        outbound_process_context (ri ACE_ENV_ARG_PARAMETER);

      }
    ACE_CATCHANY
      {
        CORBA::String_var name (this->name ());
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("%s (%P|%t) Outbound_process_context failed in  ")
                    ACE_TEXT("send_reply.\n"),
                    name.in ()));
      }
    ACE_ENDTRY;

    Server_Request_Interceptor::send_reply (ri ACE_ENV_ARG_PARAMETER);

  }

  void
  IIOP_Server_Request_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
                                               ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    ACE_TRY
      {
        outbound_process_context (ri ACE_ENV_ARG_PARAMETER);


      }
    ACE_CATCHANY
      {
        CORBA::String_var name (this->name ());
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("%s (%P|%t) Outbound_process_context failed in  ")
                    ACE_TEXT("send_exception.\n"),
                    name.in ()));

      }
    ACE_ENDTRY;

    Server_Request_Interceptor::send_exception (ri ACE_ENV_ARG_PARAMETER);
  }

  void
  IIOP_Server_Request_Interceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr ri
                                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    ACE_TRY
      {
        outbound_process_context (ri ACE_ENV_ARG_PARAMETER);
      }
    ACE_CATCHANY
      {
        CORBA::String_var name (this->name ());
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT("%s (%P|%t) Outbound_process_context failed in  ")
                    ACE_TEXT("send_other.\n"),
                    name.in ()));
      }
    ACE_ENDTRY;

    Server_Request_Interceptor::send_other (ri ACE_ENV_ARG_PARAMETER);
  }

}
