///> [Serializable]
#ifndef BUILDACTIONEX_H
#define BUILDACTIONEX_H

#include "Action.h"
#ifndef AND_H
#include "And.h"
#endif
#ifndef VECTOR2_H
#include "Vector2.h"
#endif
#ifndef MAPAREA_H
#include "MapArea.h"
#endif
#ifndef WORLDRESOURCES_H
#include "WorldResources.h"
#endif

namespace IStrategizer
{
    class CellFeature;

    ///> class=BuildActionEx
    ///> parent=Action
    class BuildActionEx : public Action
    {
        OBJECT_SERIALIZABLE_P(BuildActionEx, Action);
    public:
        BuildActionEx();
        BuildActionEx(const PlanStepParameters& p_parameters);
        bool AliveConditionsSatisfied(RtsGame& game);
        bool SuccessConditionsSatisfied(RtsGame& game);
        bool Equals(PlanStepEx* p_planStep);

    protected:
        void FreeResources(RtsGame &game);
        bool ExecuteAux(RtsGame& game, const WorldClock& p_clock);
        void HandleMessage(RtsGame& game, Message* p_msg, bool& p_consumed);
        void InitializePostConditions();
        void InitializePreConditions();

    private:
        TID _buildingId;
        TID _builderId;
        IStrategizer::MapArea _buildArea;
        bool _buildStarted;
        bool _buildIssued;
        IStrategizer::WorldResources _requiredResources;
    };
}

#endif // BUILDACTIONEX_H
