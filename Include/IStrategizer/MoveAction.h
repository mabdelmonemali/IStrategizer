///> [Serializable]
#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "Action.h"
#include "Vector2.h"

namespace IStrategizer 
{
    class GameEntity;

    ///> class=MoveAction
    ///> parent=Action
    class MoveAction:public Action
    {
    public:
        OBJECT_SERIALIZABLE(MoveAction);

        MoveAction();
        MoveAction(const PlanStepParameters& p_parameters);
        void Copy(IClonable* p_dest);
        bool AliveConditionsSatisfied(RtsGame& game);
        bool SuccessConditionsSatisfied(RtsGame& game);
        bool Equals(PlanStepEx* p_planStep);
        unsigned Hash() const { return StepTypeId() + Parameter(PARAM_ResourceId) + Parameter(PARAM_ObjectStateType); }

    protected:
        bool ExecuteAux(RtsGame& game, const WorldClock& p_clock);
        void InitializeAddressesAux() ;
        void HandleMessage(RtsGame& game, Message* p_msg, bool& p_consumed);
        void InitializePostConditions();
        void InitializePreConditions();

    private:
        TID _entityId;
        Vector2 _position;
        GameEntity* _pEntity;
    };
}

#endif// ATTACKGROUNDACTION_H 
