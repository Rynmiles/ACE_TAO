// $Id$
//
// ****              Code generated by the                 ****
// ****  Component Integrated ACE ORB (CIAO) CIDL Compiler ****
// CIAO has been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// CIDL Compiler has been developed by:
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about CIAO is available at:
//    http://www.dre.vanderbilt.edu/CIAO

#include "Sender_exec.h"

namespace CIAO_Hello_Sender_Impl
{
  char*
  Message_Impl::get_message ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "Sender sending out message: [%C]\n", component_.message_.in ()));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_i::~Sender_exec_i ()
  {
  }

  void
  Sender_exec_i::local_message (const char * local_message)
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_i::local_message ()
  {
    return CORBA::string_dup (message_.in ());
  }

  void
  Sender_exec_i::hertz (CORBA::Long hertz)
  {
    this->hertz_ = hertz;
    ACE_DEBUG ((LM_EMERGENCY, "Sender_exec_i::Hertz:%d\n", this->hertz_));
  }

  CORBA::Long
  Sender_exec_i::hertz ()
  {
    return this->hertz_;
  }

  ::Hello::COLOR_SELECTION
  Sender_exec_i::color ()
  {
    // Your code here.
    return this->color_;
  }

  void
  Sender_exec_i::color (::Hello::COLOR_SELECTION color)
  {
    this->color_ = color;
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_i::get_push_message ()
  {
    ACE_DEBUG ((LM_EMERGENCY,
                "Sender_exec.i::get_push_message called\n"));
    return ( new Message_Impl (*this) );
  }

  void
  Sender_exec_i::start ()
  {
    Hello::TimeOut_var event = new OBV_Hello::TimeOut;
    event->data ("ACE/TAO/CIAO");
    ACE_DEBUG ((LM_EMERGENCY, "Sender initiates the process.\n"));
    this->context_->push_click_out (event.in ());

    ACE_DEBUG ((LM_EMERGENCY, "My current color is:"));

    switch (this->color_)
      {
      case ::Hello::empty:
        ACE_DEBUG ((LM_EMERGENCY, "ERROR: Enum attribute initialization failed.\n"));
        break;

      case ::Hello::white:
        ACE_DEBUG ((LM_EMERGENCY, "white\n"));
        break;

      case ::Hello::red:
        ACE_DEBUG ((LM_EMERGENCY, "red\n"));
        break;

      case ::Hello::yellow:
        ACE_DEBUG ((LM_EMERGENCY, "yellow\n"));
        break;

      default:
        ACE_DEBUG ((LM_EMERGENCY, "Unknown color!\n"));

      }
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_EMERGENCY, "Sender_exec_i::set_session_context\n"));

    this->context_ =
      ::Hello::CCM_Sender_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  Sender_exec_i::configuration_complete ()
  {
    ACE_DEBUG ((LM_EMERGENCY,
                "Sender_exec_i::configuration_complete\n"));
  }

  void
  Sender_exec_i::ccm_activate ()
  {
    ACE_DEBUG ((LM_EMERGENCY,
                "Sender_exec_i::ccm_activate\n"));
  }

  void
  Sender_exec_i::ccm_passivate ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "Sender_exec_i::ccm_passivate\n"));
  }

  void
  Sender_exec_i::ccm_remove ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "Sender_exec_i::ccm_remove\n"));
  }

  SenderHome_exec_i::SenderHome_exec_i ()
  {
  }

  SenderHome_exec_i::~SenderHome_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  SenderHome_exec_i::create ()
  {
    ACE_DEBUG ((LM_EMERGENCY, "(%P|%t) creating SenderHome\n"));

    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      Sender_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_SenderHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SenderHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

