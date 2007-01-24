// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
extern PortableInterceptor::SlotId slotId;

ACE_RCSID (Bug_2510,
           Server_ORBInitializer,
           "$Id$")

Server_ORBInitializer::Server_ORBInitializer (void)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // allocate a slot
  slotId = info->allocate_slot_id ();
  ACE_DEBUG ((LM_DEBUG, "Server_ORBInitializer::post_init() : allocated slot id: %d\n", slotId));

  // Install the Echo server request interceptor
  ACE_NEW_THROW_EX (this->server_interceptor_,
                    Echo_Server_Request_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var interceptor =
    this->server_interceptor_;

  info->add_server_request_interceptor (interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
}

Echo_Server_Request_Interceptor *
Server_ORBInitializer::server_interceptor (void)
{
  return this->server_interceptor_;
}

