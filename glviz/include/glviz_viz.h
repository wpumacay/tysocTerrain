
#pragma once

// interface for our VIZ implementation
#include <viz/viz.h>
// cat1 engine functionality
#include <LApp.h>
#include <LFpsCamera.h>
#include <LFixedCamera3d.h>
#include <LLightDirectional.h>
#include <LMeshBuilder.h>
// and some specific viz wrappers
#include <glviz_kintree.h>
#include <glviz_terrainGen.h>
#include <glviz_drawable.h>
// and the current UI functionality
#include <glviz_ui.h>

#include <components/bodies.h>

namespace tysoc {

    class TCustomVisualizer : public TIVisualizer
    {

        private :

        // cat1 rendering engine resources
        engine::LApp*       m_glAppPtr;
        engine::LScene*     m_glScenePtr;
        
        // visualization wrappers
        std::vector< TCustomVizKinTree* >             m_vizKinTreeWrappers;
        std::vector< TCustomVizTerrainGenerator* >    m_vizTerrainGeneratorWrappers;

        // the UI context
        TCustomContextUI*   m_uiContextPtr;

        void _setupGlRenderingEngine();
        void _collectSingleBodies( TBody* bodyPtr );
        void _collectKinTreeAgent( agent::TAgent* agentPtr );
        void _collectTerrainGenerator( terrain::TITerrainGenerator* terrainGeneratorPtr );
        void _renderSensorReading( sensor::TISensor* sensorPtr );

        protected :

        bool _initializeInternal() override;
        void _updateInternal() override;
        void _renderUIInternal() override;
        bool _isActiveInternal() override;

        void _drawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color ) override;
        void _drawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color ) override;

        int _remapKeyInternal( int keyCode ) override;
        bool _isKeyDownInternal( int keyCode ) override;
        bool _checkSingleKeyPressInternal( int keyCode ) override;

        TIVizCamera* _createCameraInternal( const std::string& name,
                                            const std::string& type,
                                            const TVec3& pos,
                                            const TMat3& rot ) override;
        void _changeToCameraInternal( TIVizCamera* cameraPtr ) override;
        void _grabCameraFrameInternal( TIVizCamera* cameraPtr,
                                       TIVizTexture& rgbTexture,
                                       TIVizTexture& depthTexture ) override;

        TIVizLight* _createLightInternal( const std::string& name,
                                          const std::string& type,
                                          const TVec3& pos ) override;

        public :

        TCustomVisualizer( TScenario* scenarioPtr,
                           const std::string& workingDir );
        ~TCustomVisualizer(); // @CHECK: check for virtual destructors

        void addBody( TBody* bodyPtr );
        void addAgent( agent::TAgent* agentPtr );

    };

    extern "C" TIVisualizer* visualizer_createVisualizer( TScenario* scenarioPtr,
                                                          const std::string& workingDir );

}