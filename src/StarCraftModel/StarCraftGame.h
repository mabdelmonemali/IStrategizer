///> [Serializable]
#ifndef STARCRAFTGAME_H
#define STARCRAFTGAME_H

#ifndef RTSGAME_H
#include "RtsGame.h"
#endif

namespace IStrategizer
{
    ///> class=StarCraftGame
    ///> parent=RtsGame
    class StarCraftGame : public RtsGame
    {
		OBJECT_SERIALIZABLE_P(StarCraftGame, RtsGame);

    public:
        void ExecuteCommand(const char *p_cmd);
        void DisplayMessage(const char *p_msg);
        int GetMaxForceSize() const { return 12; }
        size_t GetMaxTrainingQueueCount() const { return 5; }
        unsigned GameFrame() const;

    protected:
        void InitPlayers();
        void InitMap();
        void InitEntityTypes();
        void InitResearchTypes();
        void InitRaceTypes();
        bool InitStaticData();

        GamePlayer* FetchPlayer(PlayerType p_id);
        IStrategizer::GameType* FetchEntityType(EntityClassType p_id);
        GameResearch* FetchResearch(ResearchType p_id);
    };
}

#endif // STARCRAFTGAME_H
