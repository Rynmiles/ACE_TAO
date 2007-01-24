// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Profile.h"
#include "orbsvcs/PortableGroup/PortableGroup_Request_Dispatcher.h"
#include "orbsvcs/PortableGroup/PortableGroup_Loader.h"

#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"

ACE_RCSID (PortableGroup,
           PortableGroup_Request_Dispatcher,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

PortableGroup_Request_Dispatcher::~PortableGroup_Request_Dispatcher (void)
{
}

void
PortableGroup_Request_Dispatcher::dispatch (TAO_ORB_Core *orb_core,
                                            TAO_ServerRequest &request,
                                            CORBA::Object_out forward_to
                                            ACE_ENV_ARG_DECL)
{
  // Check if the request contains a tagged profile
  if (request.profile ().discriminator () == GIOP::ProfileAddr)
    {
      // Get the IOP::Tagged profile.
      const IOP::TaggedProfile &tagged_profile =
        request.profile ().tagged_profile ();
      PortableGroup::TagGroupTaggedComponent group;

      // Extract a Group ID from the profile if it exists.
      int result = TAO_UIPMC_Profile::extract_group_component (tagged_profile,
                                                               group);
      if (result == 0)
        {
          // Got a group.  Dispatch based on the group ID
          this->group_map_.dispatch (&group,
                                     orb_core,
                                     request,
                                     forward_to
                                     ACE_ENV_ARG_PARAMETER);

          return;
        }

      // Else, fall through and dispatch based on object key like normal.
    }

  // Dispatch based on object key.
  orb_core->adapter_registry ()->dispatch (request.object_key (),
                                           request,
                                           forward_to
                                           ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
