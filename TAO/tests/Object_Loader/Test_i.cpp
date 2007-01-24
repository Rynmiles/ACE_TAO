// $Id$

#include "Test_i.h"

ACE_RCSID(Object_Loader, Test_i, "$Id$")

CORBA::Long
Test_i::instance_count_ = 0;

Test_i::Test_i (PortableServer::POA_ptr poa)
  :  poa_ (PortableServer::POA::_duplicate (poa))
{
  Test_i::instance_count_++;
}

CORBA::Long
Test_i::instance_count (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Test_i::instance_count_;
}

void
Test_i::destroy (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_var poa =
    this->_default_POA ();
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
  poa->deactivate_object (oid.in () ACE_ENV_ARG_PARAMETER);
}

PortableServer::POA_ptr
Test_i::_default_POA (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
