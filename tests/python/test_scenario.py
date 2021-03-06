#!/usr/bin/env python

import loco
import tinymath as tm
import numpy as np
import gc

def test_scenario_constructor() :
    scenario = loco.sim.Scenario()
    scenario.Initialize()
    scenario.PreStep()
    scenario.PostStep()
    scenario.Reset()
    assert ( scenario.HasSingleBodyNamed( 'foo' ) == False ), 'test_scenario_constructor >>> method \'HasSingleBodyNamed\' doesn\'t return correct value'
    assert ( scenario.GetSingleBodyByName( 'foo' ) == None ), 'test_scenario_constructor >>> method \'GetSingleBodyByName\' doesn\'t return correct value'
    assert ( len( scenario.GetSingleBodiesList() ) == 0 ), 'test_scenario_constructor >>> method \'GetSingleBodiesList\' doesn\'t return correct value'
    assert ( scenario.GetNumSingleBodies() == 0 ), 'test_scenario_constructor >>> method \'GetNumSingleBodies\' doesn\'t return correct value'
    print( scenario )

def test_scenario_primitives() :
    vis_data = loco.sim.VisualData()
    vis_data.type = loco.sim.ShapeType.CAPSULE
    vis_data.size = [ 0.1, 0.2, 0.1 ]
    col_data = loco.sim.CollisionData()
    col_data.type = loco.sim.ShapeType.CAPSULE
    col_data.size = [ 0.1, 0.2, 0.1 ]

    body_data = loco.sim.BodyData()
    body_data.dyntype = loco.sim.DynamicsType.DYNAMIC
    body_data.collision = col_data
    body_data.visual = vis_data

    body_obj = loco.sim.SingleBody( "body_0", body_data, [ 1.0, 1.0, 1.0 ], np.identity( 3 ) )
    scenario = loco.sim.Scenario()
    scenario.AddSingleBody( body_obj )
    assert ( len( gc.get_referrers( scenario ) ) == 1 ), 'test_scenario_primitives >>> num-referrers to scenario should have remain constant'
    assert ( scenario.HasSingleBodyNamed( 'body_0' ) == True ), 'test_scenario_primitives >>> issue with method \'HasSingleBodyNamed\''
    assert ( scenario.GetSingleBodyByName( 'body_0' ) != None ), 'test_scenario_primitives >>> issue with method \'GetSingleBodyByName\''
    assert ( len( scenario.GetSingleBodiesList() ) == 1 ), 'test_scenario_primitives >>> issue with method \'GetSingleBodiesList\''
    assert ( scenario.GetSingleBodiesList()[0].name == 'body_0' ), 'test_scenario_primitives >>> issue with values returned by \'GetSingleBodiesList\''
    assert ( scenario.GetNumSingleBodies() == 1 ), 'test_scenario_primitives >>> issue with method \'GetNumSingleBodies\''
    print( scenario )

if __name__ == '__main__' :
    _ = input( 'Press ENTER to start test : test_scenario_constructor' )
    test_scenario_constructor()

    _ = input( 'Press ENTER to start test : test_scenario_primitives' )
    test_scenario_primitives()

    _ = input( 'Press ENTER to continue ...' )