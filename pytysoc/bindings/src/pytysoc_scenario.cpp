
#include <pytysoc_scenario.h>

namespace py = pybind11;

namespace pytysoc
{

    PyScenario::PyScenario()
    {
        m_scenarioPtr = new tysoc::TScenario();
    }

    PyScenario::~PyScenario()
    {
        m_pyCoreAgentsMap.clear();

        //for ( size_t q = 0; q < m_pyCoreAgents.size(); q++ )
        //{
        //    delete m_pyCoreAgents[q];
        //    m_pyCoreAgents[q] = NULL;
        //}
        m_pyCoreAgents.clear();

        if ( m_scenarioPtr )
        {
            delete m_scenarioPtr;
            m_scenarioPtr = NULL;
        }
    }

    void PyScenario::addBody( PyBody* pyBodyPtr )
    {
        assert( m_scenarioPtr );
        assert( pyBodyPtr->ptr() );

        if ( m_pyBodiesMap.find( pyBodyPtr->name() ) != m_pyBodiesMap.end() )
        {
            std::cout << "WARNING> tried to add an existing body with name: " << pyBodyPtr->name() << std::endl;
            return;
        }

        m_scenarioPtr->addBody( pyBodyPtr->ptr() );

        m_pyBodies.push_back( pyBodyPtr );
        m_pyBodiesMap[ pyBodyPtr->name() ] = pyBodyPtr;
    }

    void PyScenario::addAgent( PyCoreAgent* pyCoreAgentPtr )
    {
        if ( !m_scenarioPtr )
            return;

        if ( pyCoreAgentPtr->name() == "undefined" )
        {
            std::cout << "ERROR> tried to add a pyCoreAgent with no wrapped kintree agent" << std::endl;
            return;
        }

        if ( m_pyCoreAgentsMap.find( pyCoreAgentPtr->name() ) != m_pyCoreAgentsMap.end() )
        {
            std::cout << "WARNING> tried to add an existing (same name) pyCoreAgent" << std::endl;
            return;
        }

        m_scenarioPtr->addAgent( pyCoreAgentPtr->ptr() );

        m_pyCoreAgents.push_back( pyCoreAgentPtr );
        m_pyCoreAgentsMap[ pyCoreAgentPtr->name() ] = pyCoreAgentPtr;
    }

    void PyScenario::addTerrainGen( PyTerrainGen* pyTerrainGenPtr )
    {
        if ( !m_scenarioPtr )
            return;

        if ( pyTerrainGenPtr->name() == "undefined" )
        {
            std::cout << "ERROR> tried to add a pyTerrainGen with no wrapped terrainGen" << std::endl;
            return;
        }

        if ( m_pyTerrainGensMap.find( pyTerrainGenPtr->name() ) != m_pyTerrainGensMap.end() )
        {
            std::cout << "WARNING> tried to add an existing (same name) pyTerrainGen" << std::endl;
            return;
        }

        m_scenarioPtr->addTerrainGenerator( pyTerrainGenPtr->ptr() );

        m_pyTerrainGens.push_back( pyTerrainGenPtr );
        m_pyTerrainGensMap[ pyTerrainGenPtr->name() ] = pyTerrainGenPtr;
    }

    void PyScenario::addSensor( PySensor* pySensorPtr )
    {
        if ( !m_scenarioPtr )
            return;

        if ( pySensorPtr->name() == "undefined" )
        {
            std::cout << "ERROR> tried to add a pySensor with no wrapped sensor" << std::endl;
            return;
        }

        if ( m_pySensorsMap.find( pySensorPtr->name() ) != m_pySensorsMap.end() )
        {
            std::cout << "WARNING> tried to add an existing (same name) pySensor" << std::endl;
            return;
        }

        m_scenarioPtr->addSensor( pySensorPtr->ptr() );

        m_pySensors.push_back( pySensorPtr );
        m_pySensorsMap[ pySensorPtr->name() ] = pySensorPtr;
    }

    PyBody* PyScenario::getBodyByName( const std::string& name )
    {
        if ( m_pyBodiesMap.find( name ) == m_pyBodiesMap.end() )
        {
            std::cout << "WARNING> body: " << name << " not found in scenario" << std::endl;
            return nullptr;
        }

        return m_pyBodiesMap[ name ];
    }

    PyCoreAgent* PyScenario::getAgentByName( const std::string& name )
    {
        if ( m_pyCoreAgentsMap.find( name ) == m_pyCoreAgentsMap.end() )
        {
            std::cout << "WARNING> agent: " << name << " not found in scenario" << std::endl;
            return nullptr;
        }

        return m_pyCoreAgentsMap[ name ];
    }

    PyTerrainGen* PyScenario::getTerrainGenByName( const std::string& name )
    {
        if ( m_pyTerrainGensMap.find( name ) == m_pyTerrainGensMap.end() )
        {
            std::cout << "WARNING> terrainGen: " << name << " not found in scenario" << std::endl;
            return nullptr;
        }

        return m_pyTerrainGensMap[ name ];
    }

    PySensor* PyScenario::getSensorByName( const std::string& name )
    {
        if ( m_pySensorsMap.find( name ) == m_pySensorsMap.end() )
        {
            std::cout << "WARNING> sensor: " << name << " not found in scenario" << std::endl;
            return nullptr;
        }

        return m_pySensorsMap[ name ];
    }

    std::vector< PyBody* > PyScenario::getBodies()
    {
        return m_pyBodies;
    }

    std::vector< PyCoreAgent* > PyScenario::getAgents()
    {
        return m_pyCoreAgents;
    }

    std::vector< PyTerrainGen* > PyScenario::getTerrainGens()
    {
        return m_pyTerrainGens;
    }

    std::vector< PySensor* > PyScenario::getSensors()
    {
        return m_pySensors;
    }

    std::map< std::string, PyBody* > PyScenario::getBodiesMap()
    {
        return m_pyBodiesMap;
    }

    std::map< std::string, PyCoreAgent* > PyScenario::getAgentsMap()
    {
        return m_pyCoreAgentsMap;
    }

    std::map< std::string, PyTerrainGen* > PyScenario::getTerrainGensMap()
    {
        return m_pyTerrainGensMap;
    }

    std::map< std::string, PySensor* > PyScenario::getSensorsMap()
    {
        return m_pySensorsMap;
    }

    tysoc::TScenario* PyScenario::ptr()
    {
        return m_scenarioPtr;
    }

}