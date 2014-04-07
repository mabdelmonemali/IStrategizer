#ifndef GAMETYPE_H
#define GAMETYPE_H

#ifndef STATICCOMPONENT_H
#include "StaticComponent.h"
#endif

#ifndef METADATA_H
#include "MetaData.h"
#endif
#include <memory>

namespace IStrategizer
{
    class WorldResources;
    class GameDependency;

    class GameType : public StaticComponent
    {
    protected:
        EntityClassType m_id;
        WorldResources* m_requiredResources;
        int             m_attributes[COUNT(EntityClassAttribute)];

    protected:
        void            Attr(EntityClassAttribute p_attrId, int p_val) { m_attributes[INDEX(p_attrId, EntityClassAttribute)] = p_val; }
    public:
                        GameType() : m_requiredResources(nullptr) {}
                        ~GameType();
        int             Id() const { return m_id; }
        WorldResources* RequiredResources() const { return m_requiredResources; }
        int             Attr(EntityClassAttribute p_attrId) const { return m_attributes[INDEX(p_attrId, EntityClassAttribute)]; }
    };
    
    typedef std::tr1::shared_ptr<GameType> GameTypeStrongPtr;
}

#endif // GAMETYPE_H
