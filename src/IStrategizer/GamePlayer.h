///> [Serializable]
#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include <vector>
#include "EngineData.h"
#include "IMessagePumpObserver.h"
#include "MapArea.h"
#include "ISerializable.h"
#include "GameTechTree.h"
#include "GameRace.h"
#include "EngineObject.h"

namespace IStrategizer
{
    enum PlayerType;
    enum ResearchType;
    class GameEntity;
    class PlayerResources;
    class Message;
    class RtsGame;
    class SimilarityWeightModel;

    typedef Serialization::SMap<TID, GameEntity*> EntitiesMap;

    ///> class=GamePlayer
    class GamePlayer : public EngineObject
    {
        OBJECT_SERIALIZABLE(GamePlayer, &m_isOnline, &m_type, &m_raceId, &m_pResources, &m_pTechTree, &m_entities);
    public:
        GamePlayer(TID raceId);
        virtual ~GamePlayer();
        void Entities(std::vector<TID>& p_entityIds);
        void Entities(EntityClassType p_typeId, std::vector<TID> &p_entityIds);
        void GetBases(std::vector<TID> &p_basesIds);
        GameEntity* GetEntity(TID p_id);
        MapArea GetColonyMapArea();
        virtual void SetOffline(RtsGame* pBelongingGame) = 0;
        virtual const GameRace* Race() const = 0;
        void NotifyMessegeSent(Message* p_pMessage);
        void Init();
        float Distance(const GamePlayer* pOther, const SimilarityWeightModel* pModel) const { return 0.0f; }
		int Attr(PlayerAttribute attribute);

        PlayerResources* Resources() { _ASSERTE(m_pResources != nullptr); return m_pResources;}
        const PlayerResources* Resources() const { _ASSERTE(m_pResources != nullptr); return m_pResources;}
        GameTechTree* TechTree() const { _ASSERTE(m_pTechTree != nullptr); return m_pTechTree; }
        // Count number of entities which have their type attribute [attr] equals [val]
        int CountEntityTypes(_In_ EntityClassAttribute attr, _In_ int val) const;

    protected:
        virtual GameEntity* FetchEntity(TID p_id) = 0;
        virtual void Finalize();
        virtual void OnEntityRenegade(Message* p_pMessage);
        virtual void OnEntityCreate(Message* p_pMessage);
        virtual void OnEntityDestroy(Message* p_pMessage);

        ///> type=bool
        bool m_isOnline;
        ///> type=int
        PlayerType m_type;
        ///> type=int
        TID m_raceId;
        ///> type=PlayerResources*
        PlayerResources *m_pResources;
        ///> type=GameTechTree*
        GameTechTree *m_pTechTree;
        ///> type=map(pair(int,GameEntity*))
        EntitiesMap m_entities;

        MapArea m_colonyCenter;
    };
}

#endif // GAMEPLAYER_H
