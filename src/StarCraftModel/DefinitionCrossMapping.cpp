#include "DefinitionCrossMapping.h"
#include "BWAPI.h"
#include <sstream>
#include <fstream>

using namespace BWAPI;
using namespace std;

const int TechIdOffset = 127;

DefinitionCrossMapping::DefinitionCrossMapping() : m_initialized(false) {}
//---------------------------------------------------------------------------------------------
void DefinitionCrossMapping::Init()
{
    if (m_initialized)
    {
        // Because we depend on player actual Id in the game, and this class is singleton
        // The starcraft game may change player ids from game run to another
        PlayerMapping.Clear();
        InitPlayers();
        return;
    }

    InitEntities();
    InitUpgrades();
    InitTeches();
    InitPlayers();
    InitActions();
    InitEntityIdents();
    InitUpgradeIdents();
    InitTechIdents();
    InitActions();

    m_initialized = true;
}
//---------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitEntities()
{
    vector< pair<TID, EntityClassType> > entities;
    const char *name;

    int currentId = (int)ECLASS_START;
    for(UnitType::set::iterator i = UnitTypes::allUnitTypes().begin();
        i != UnitTypes::allUnitTypes().end();
        ++i)
    {
        entities.push_back(make_pair((*i).getID(), (EntityClassType)currentId));

        name = (*i).getName().c_str();
        Enums[currentId] = _strdup(name);

        ++currentId;
    }

    EntityMapping = CrossMap<TID, EntityClassType>(entities);
}
//---------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitUpgrades()
{
    vector< pair<TID, ResearchType> > upgrades;
    const char *name;

    int currentId = (int)RESEARCH_START;
    _ASSERTE(UpgradeTypes::allUpgradeTypes().size() < (COUNT(ResearchType) / 2));

    for(UpgradeType::set::iterator i = UpgradeTypes::allUpgradeTypes().begin();
        i != UpgradeTypes::allUpgradeTypes().end();
        ++i)
    {
        upgrades.push_back(make_pair((*i).getID(), (ResearchType)currentId));

        name = (*i).getName().c_str();
        Enums[currentId] = _strdup(name);

        ++currentId;
    }

    UpgradeMapping = CrossMap<TID, ResearchType>(upgrades);
}
//--------------------------------------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitTeches()
{
    vector< pair<TID, ResearchType> > m_teches;
    const char *name;

    int currentId = (int)RESEARCH_START + TechIdOffset;
    _ASSERTE(TechTypes::allTechTypes().size() < (COUNT(ResearchType) / 2));

    for(TechType::set::iterator i = TechTypes::allTechTypes().begin();
        i != TechTypes::allTechTypes().end();
        ++i)
    {
        m_teches.push_back(make_pair((*i).getID(), (ResearchType)currentId));

        name = (*i).getName().c_str();
        Enums[currentId] = _strdup(name);

        ++currentId;
    }

    TechMapping = CrossMap<TID, ResearchType>(m_teches);
}
//----------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitPlayers()
{
    bool playerFound = false;
    bool selfPlayerIsComputer = false;
    TID selfID = DONT_CARE;
    vector<pair<TID, IStrategizer::PlayerType>> m_players;
    const Playerset &players = Broodwar->getPlayers();

    // Finding PLAYER_Self
    playerFound = false;
    TID selfType = selfPlayerIsComputer ? PlayerTypes::Computer.getID() : PlayerTypes::Player.getID();
    for (BWAPI::Player pPlayer : players)
    {
        if (pPlayer->getType().getID() == selfType)
        {
            m_players.push_back(make_pair(pPlayer->getID(), PLAYER_Self));
            selfID = pPlayer->getID();
            playerFound = true;
            break;
        }
    }
    _ASSERTE(playerFound);

    // Finding the enemy (PLAYER_Enemy)
    playerFound = false;
    for (BWAPI::Player pPlayer : players)
    {
        if (pPlayer->isEnemy(Broodwar->getPlayer(selfID)))
        {
            m_players.push_back(make_pair(pPlayer->getID(), PLAYER_Enemy));
            playerFound = true;
            break;
        }
    }

    // Finding PLAYER_Neutral
    playerFound = false;
    for (BWAPI::Player pPlayer : players)
    {
        if (pPlayer->isNeutral())
        {
            m_players.push_back(make_pair(pPlayer->getID(), PLAYER_Neutral));
            playerFound = true;
            break;
        } 
    }

    // We don't handle more that 3 players for now.
    _ASSERTE(m_players.size() == 3);
    PlayerMapping = CrossMap<TID, IStrategizer::PlayerType>(m_players);
}
//----------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitEntityIdents()
{
    vector< pair<TID, TName> > entities;

    for(UnitType::set::iterator i = UnitTypes::allUnitTypes().begin();
        i != UnitTypes::allUnitTypes().end();
        ++i)
    {
        entities.push_back(make_pair((*i).getID(), (*i).getName()));
    }

    EntityIdentMapping = CrossMap<TID, TName>(entities);
}
//----------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitUpgradeIdents()
{
    vector< pair<TID, TName> > m_upgrades;

    for(UpgradeType::set::iterator i = UpgradeTypes::allUpgradeTypes().begin();
        i != UpgradeTypes::allUpgradeTypes().end();
        ++i)
    {
        m_upgrades.push_back(make_pair((*i).getID(), (*i).getName()));
    }

    UpgradeIdentMapping = CrossMap<TID, TName>(m_upgrades);
}
//----------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitTechIdents()
{
    vector< pair<TID, TName> > m_teches;

    for(TechType::set::iterator i = TechTypes::allTechTypes().begin();
        i != TechTypes::allTechTypes().end();
        ++i)
    {
        m_teches.push_back(make_pair((*i).getID(), (*i).getName()));
    }

    TechIdentMapping = CrossMap<TID, TName>(m_teches);
}
//----------------------------------------------------------------------------------------------
bool DefinitionCrossMapping::ExportGameIds(string p_exportPath)
{
    fstream pen;
    stringstream stream;
    const char* name;
    pen.open(p_exportPath.c_str(), ios::out);

    if (!pen.is_open())
        return false;

    // TODO: Looping can be smart than this by looping only on required types
    // without looping on the while ENUMS_SIZE
    for (unsigned id = 0; id < ENUMS_SIZE; ++id)
    {
        if (BELONG(EntityClassType, id) ||
            BELONG(ResearchType, id))
        {
            name = Enums[id];

            // Has a name defined
            if (name != nullptr)
            {
                stream << id << ' ' << name << endl;
            }
        }
    }

    pen << stream.str();
    pen.close();

    return true;
}
//----------------------------------------------------------------------------------------------
bool DefinitionCrossMapping::ExportAllIds(string p_exportPath)
{
    fstream pen;
    stringstream stream;
    const char* name;
    pen.open(p_exportPath.c_str(), ios::out);

    if (!pen.is_open())
        return false;

    for (unsigned id = 0; id < ENUMS_SIZE; ++id)
    {
        name = Enums[id];

        // Has a name defined
        if (name != nullptr)
        {
            stream << id << ' ' << name << endl;
        }
    }

    pen << stream.str();
    pen.close();

    return true;
}
//----------------------------------------------------------------------------------------------
void DefinitionCrossMapping::EntityTypes(vector<EntityClassType>& p_entityTypes)
{
    EntityMapping.SecondValues(p_entityTypes);
}
//----------------------------------------------------------------------------------------------
void DefinitionCrossMapping::ResearchTypes(vector<IStrategizer::ResearchType>& p_researchTypes)
{
    UpgradeMapping.SecondValues(p_researchTypes);
    TechMapping.SecondValues(p_researchTypes, true);
}
//----------------------------------------------------------------------------------------------
void DefinitionCrossMapping::InitActions()
{
    vector< pair<TID, ActionType> > actions;

    actions.push_back(make_pair(Orders::Train.getID(), ACTIONEX_Train));
    
    actions.push_back(make_pair(Orders::AttackMove.getID(), ACTIONEX_AttackGround));
    actions.push_back(make_pair(Orders::AttackUnit.getID(), ACTIONEX_AttackEntity));

    actions.push_back(make_pair(Orders::ResearchTech.getID(), ACTIONEX_Research));
    actions.push_back(make_pair(Orders::Upgrade.getID(), ACTIONEX_Research));

    actions.push_back(make_pair(Orders::PlaceBuilding.getID(), ACTIONEX_Build));
    actions.push_back(make_pair(Orders::PlaceAddon.getID(), ACTIONEX_Build));
    
    actions.push_back(make_pair(Orders::MoveToMinerals.getID(), ACTIONEX_GatherResource));
    actions.push_back(make_pair(Orders::MiningMinerals.getID(), ACTIONEX_GatherResource));
    actions.push_back(make_pair(Orders::WaitForMinerals.getID(), ACTIONEX_GatherResource));
    actions.push_back(make_pair(Orders::ReturnMinerals.getID(), ACTIONEX_GatherResource));
    
    actions.push_back(make_pair(Orders::MoveToGas.getID(), ACTIONEX_GatherResource));
    actions.push_back(make_pair(Orders::HarvestGas.getID(), ACTIONEX_GatherResource));
    actions.push_back(make_pair(Orders::WaitForGas.getID(), ACTIONEX_GatherResource));
    actions.push_back(make_pair(Orders::ReturnGas.getID(), ACTIONEX_GatherResource));

    ActionMapping = CrossMap<TID, IStrategizer::ActionType>(actions);
}

