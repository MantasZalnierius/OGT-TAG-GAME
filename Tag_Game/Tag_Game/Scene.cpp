#include "Scene.h"

Scene::Scene(GameState t_nextGameState)
{
}

const bool& Scene::isEnded() const
{
    return m_isEnded;
}

const GameState& Scene::getNewGameState() const
{
    return m_nextGameState;
}

int Scene::getServerOrClient() const
{
    return m_hostOrPlayer;
}
