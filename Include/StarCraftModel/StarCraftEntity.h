#ifndef STARCRAFTENTITY_H
#define STARCRAFTENTITY_H

#ifndef GAMEENTITY_H
#include "GameEntity.h"
#endif

#include "BWAPI.h"

namespace IStrategizer
{
    enum ObjectStateType;
    enum EntityClassType;
    enum PlayerType;
}

namespace StarCraftModel
{
    class StarCraftEntity: public IStrategizer::GameEntity
    {
    public:
        StarCraftEntity(BWAPI::Unit p_unit);
        std::string ToString() const;
        IStrategizer::Vector2 GetPosition() const;
        bool IsTraining(IStrategizer::TID p_traineeId) const;

        bool Research(IStrategizer::ResearchType p_researchId);
        bool Build(IStrategizer::EntityClassType p_buildingClassId, IStrategizer::Vector2 p_position);
        bool AttackGround(IStrategizer::Vector2 p_position);
        bool AttackEntity(IStrategizer::TID p_targetEntityObjectId);
        bool Train(IStrategizer::EntityClassType p_entityClassId);
        bool Move(IStrategizer::Vector2 p_position);
        bool IsNull();
        IClonable* Clone();
        void Copy(IClonable* pDest);

    protected:
        StarCraftEntity() {}
        int FetchAttr(IStrategizer::EntityObjectAttribute attrId) const;
        IStrategizer::ObjectStateType FetchState() const;

    private:
        BWAPI::Unit    m_pUnit;
    };
}

#endif // STARCRAFTENTITY_H