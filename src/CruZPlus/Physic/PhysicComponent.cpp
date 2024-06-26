#include "PhysicComponent.h"
#include "CruZPlus/BodyFactory.h"

namespace CruZ
{
    PhysicComponent::PhysicComponent()
    {
    }

    PhysicComponent::~PhysicComponent()
    {
        INS(BodyFactory)->DestroyBody(*m_body);
    }
}
