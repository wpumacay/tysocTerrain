
#include <loco_visualizer_opengl.h>

namespace loco
{
    TOpenGLVisualizer::TOpenGLVisualizer( TScenario* scenarioRef,
                                          size_t windowWidth,
                                          size_t windowHeight,
                                          bool windowResizable,
                                          bool renderOffscreen )
        : TIVisualizer( scenarioRef, windowWidth, windowHeight, windowResizable, renderOffscreen )
    {
    #ifndef LOCO_OPENGL_VISUALIZER_HEADLESS
        m_backendId = "openglviz_glfw";
    #else
        m_backendId = "openglviz_egl";
    #endif
        _SetupRenderingEngine( windowWidth, windowHeight, windowResizable, renderOffscreen );

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TOpenGLVisualizer @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TOpenGLVisualizer @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TOpenGLVisualizer::~TOpenGLVisualizer()
    {
        m_glApplication = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( TLogger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TOpenGLVisualizer @ {0}", loco::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TOpenGLVisualizer @ " << loco::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TOpenGLVisualizer::_ChangeScenarioInternal()
    {
        if ( !m_glApplication )
            return;

        _CollectSingleBodies();
        //// _CollectCompounds();
        //// _CollectKintreeAgents();
        //// _CollectKintreeSensors();
        //// _CollectTerrainGenerators();
    }

    void TOpenGLVisualizer::_InitializeInternal()
    {
        _CollectSingleBodies();
        //// _CollectCompounds();
        //// _CollectKintreeAgents();
        //// _CollectKintreeSensors();
        //// _CollectTerrainGenerators();
    }

    void TOpenGLVisualizer::_SetupRenderingEngine( size_t windowWidth,
                                                   size_t windowHeight,
                                                   bool windowResizable,
                                                   bool renderOffscreen )
    {
        auto windowProperties = engine::CWindowProps();
        windowProperties.width = windowWidth;
        windowProperties.height = windowHeight;
        windowProperties.title = "OpenGL visualizer";
        windowProperties.clearColor = { 0.6f, 0.659f, 0.690f, 1.0f };
        windowProperties.resizable = windowResizable;

    #ifndef LOCO_OPENGL_VISUALIZER_HEADLESS
        auto imguiProperties = engine::CImGuiProps();
        imguiProperties.useDockingSpace = true;
        imguiProperties.useDockingSpacePassthrough = true;
        imguiProperties.useAutosaveLayout = true;
        imguiProperties.fileLayout = "";

        // Create application with both window-props and imgui-props (there's imgui support in this mode)
        m_glApplication = std::make_unique<engine::CApplication>( windowProperties,
                                                                  imguiProperties );
        m_glApplication->setOffscreenRendering( renderOffscreen );
    #else
        m_glApplication = std::make_unique<engine::CApplication>( windowProperties );
        m_glApplication->setOffscreenRendering( true );
    #endif /* LOCO_OPENGL_VISUALIZER_HEADLESS */

        // Create a default camera for the scenario
        auto viz_cameraRef = CreateCamera( "main_orbit",
                                           eVizCameraType::ORBIT,
                                           { 4.0f, 4.0f, 4.0f },
                                           { 0.0f, 0.0f, 0.0f } );

        // Create a default light for the scenario
        auto viz_lightRef = CreateLight( "main_light",
                                         eVizLightType::DIRECTIONAL,
                                         { 0.5f, 0.5f, 0.5f },
                                         { 0.8f, 0.8f, 0.8f },
                                         { 0.8f, 0.8f, 0.8f } );
        viz_lightRef->SetDirection( { -1.0f, -1.0f, -1.0f } );
        viz_lightRef->SetCastShadows( true );

        auto gl_cameraRef = dynamic_cast<TOpenGLCameraAdapter*>( viz_cameraRef->adapter() )->gl_camera();

        m_glApplication->renderOptions().useSkybox = false;
        m_glApplication->renderOptions().useBlending = true;
        m_glApplication->renderOptions().useShadowMapping = true;
        m_glApplication->renderOptions().pcfCount = 0;
        m_glApplication->renderOptions().shadowMapRangeConfig.type = engine::eShadowRangeType::FIXED_USER;
        m_glApplication->renderOptions().shadowMapRangeConfig.worldUp = { 0.0f, 0.0f, 1.0f };
        m_glApplication->renderOptions().shadowMapRangeConfig.cameraPtr = gl_cameraRef; // @firsttodo: should check this on engine instead of user side
    }

    void TOpenGLVisualizer::_UpdateInternal()
    {
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

    #ifndef LOCO_OPENGL_VISUALIZER_HEADLESS
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_G ) )
        {
            LOCO_CORE_TRACE( "Toggling ui state" );
            m_glApplication->setGuiActive( !m_glApplication->guiActive() );
        }
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_U ) )
        {
            LOCO_CORE_TRACE( "Toggling ui-utils state" );
            m_glApplication->setGuiUtilsActive( !m_glApplication->guiUtilsActive() );
        }
    #endif

        m_glApplication->update();
        m_glApplication->begin();
        m_glApplication->render();
        m_glApplication->end();
    }

    void TOpenGLVisualizer::_ResetInternal()
    {
        // Nothing extra to reset
    }

    void TOpenGLVisualizer::_DrawLineInternal( const TVec3& start, const TVec3& end, const TVec3& color )
    {
        engine::CDebugDrawer::DrawLine( start, end, color );
    }

    void TOpenGLVisualizer::_DrawAABBInternal( const TVec3& aabbMin, const TVec3& aabbMax, const TMat4& aabbWorldTransform, const TVec3& color )
    {
        engine::CDebugDrawer::DrawAABB( aabbMin, aabbMax, aabbWorldTransform, color );
    }

    void TOpenGLVisualizer::_DrawSolidCylinderInternalX( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        engine::CDebugDrawer::DrawSolidCylinder( radius, height, engine::eAxis::X, transform, color );
    }

    void TOpenGLVisualizer::_DrawSolidCylinderInternalY( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        engine::CDebugDrawer::DrawSolidCylinder( radius, height, engine::eAxis::Y, transform, color );
    }

    void TOpenGLVisualizer::_DrawSolidCylinderInternalZ( float radius, float height, const TMat4& transform, const TVec4& color )
    {
        engine::CDebugDrawer::DrawSolidCylinder( radius, height, engine::eAxis::Z, transform, color );
    }

    void TOpenGLVisualizer::_DrawSolidArrowInternalX( float length, const TMat4& transform, const TVec4& color)
    {
        engine::CDebugDrawer::DrawSolidArrow( length, engine::eAxis::X, transform, color );
    }

    void TOpenGLVisualizer::_DrawSolidArrowInternalY( float length, const TMat4& transform, const TVec4& color)
    {
        engine::CDebugDrawer::DrawSolidArrow( length, engine::eAxis::Y, transform, color );
    }

    void TOpenGLVisualizer::_DrawSolidArrowInternalZ( float length, const TMat4& transform, const TVec4& color)
    {
        engine::CDebugDrawer::DrawSolidArrow( length, engine::eAxis::Z, transform, color );
    }

    bool TOpenGLVisualizer::_IsActiveInternal() const
    {
        if ( !m_glApplication )
            return false;

        return m_glApplication->active();
    }

    bool TOpenGLVisualizer::_IsKeyDownInternal( int key ) const
    {
        return engine::CInputManager::IsKeyDown( key );
    }

    bool TOpenGLVisualizer::_CheckSingleKeyPressInternal( int key ) const
    {
        return engine::CInputManager::CheckSingleKeyPress( key );
    }

    bool TOpenGLVisualizer::_IsMouseDownInternal( int button ) const
    {
        return engine::CInputManager::IsMouseDown( button );
    }

    TVec2 TOpenGLVisualizer::_GetCursorPositionInternal() const
    {
        return engine::CInputManager::GetCursorPosition();
    }

    void TOpenGLVisualizer::_CreateCameraInternal( TVizCamera* cameraRef )
    {
        if ( !m_glApplication )
        {
            LOCO_CORE_ERROR( "TOpenGLVisualizer::_CreateCameraInternal >>> haven't initialized tiny-renderer gl-application" );
            return;
        }

        auto gl_camera = loco::glviz::CreateCamera( cameraRef );
        if ( !gl_camera )
        {
            LOCO_CORE_ERROR( "TOpenGLVisualizer::_CreateCameraInternal >>> couldn't create camera named {0}", cameraRef->name() );
            return;
        }

        // Add engine-camera to the scene
        auto gl_cameraRef = m_glApplication->scene()->AddCamera( std::move( gl_camera ) );
        // Create camera adapter and link it to the adaptee (user camera)
        auto gl_cameraAdapter = std::make_unique<TOpenGLCameraAdapter>( cameraRef, gl_cameraRef );
        cameraRef->SetAdapter( gl_cameraAdapter.get() );
        // Keep ownership of the camera adapter
        m_vizCameraAdapters.push_back( std::move( gl_cameraAdapter ) );
    }

    void TOpenGLVisualizer::_CreateLightInternal( TVizLight* lightRef )
    {
        if ( !m_glApplication )
        {
            LOCO_CORE_ERROR( "TOpenGLVisualizer::_CreateLightInternal >>> haven't initialized tiny-renderer gl-application" );
            return;
        }

        auto gl_light = loco::glviz::CreateLight( lightRef );
        if ( !gl_light )
        {
            LOCO_CORE_ERROR( "TOpenGLVisualizer::_CreateLightInternal >>> couldn't create light named {0}", lightRef->name() );
            return;
        }

        // Add engine-light to the scene
        auto gl_lightRef = m_glApplication->scene()->AddLight( std::move( gl_light ) );
        // Create light adapter and link it to the adaptee (user light)
        auto gl_lightAdapter = std::make_unique<TOpenGLLightAdapter>( lightRef, gl_lightRef );
        lightRef->SetAdapter( gl_lightAdapter.get() );
        // Keep ownership of the light adapter
        m_vizLightAdapters.push_back( std::move( gl_lightAdapter ) );
    }

    void TOpenGLVisualizer::_ChangeCurrentCameraInternal( TVizCamera* cameraRef )
    {
        if ( !m_glApplication )
        {
            LOCO_CORE_ERROR( "TOpenGLVisualizer::_ChangeCurrentCameraInternal >>> haven't initialized tiny-renderer gl-application" );
            return;
        }

        m_glApplication->scene()->ChangeCurrentCamera( cameraRef->name() );
        // Tell the engine to use this camera for shadow-range calculations (if enabled) @firsttodo: should check this on engine instead of user side
        auto gl_cameraRef = dynamic_cast<TOpenGLCameraAdapter*>( cameraRef->adapter() )->gl_camera();
        m_glApplication->renderOptions().shadowMapRangeConfig.cameraPtr = gl_cameraRef;
    }

    void TOpenGLVisualizer::_ChangeCurrentLightInternal( TVizLight* lightRef )
    {
        if ( !m_glApplication )
        {
            LOCO_CORE_ERROR( "TOpenGLVisualizer::_ChangeCurrentLightInternal >>> haven't initialized tiny-renderer gl-application" );
            return;
        }

        m_glApplication->scene()->ChangeCurrentLight( lightRef->name() );
    }

    void TOpenGLVisualizer::_CollectSingleBodies()
    {
        auto singleBodiesList = m_scenarioRef->GetSingleBodiesList();
        for ( auto singleBody : singleBodiesList )
        {
            auto collision = singleBody->collision();
            if ( collision )
            {
                auto gl_renderable = loco::glviz::CreateRenderable( collision->data() );
                if ( gl_renderable )
                {
                    // Add engine-renderable to the scene
                    auto gl_renderableRef = m_glApplication->scene()->AddRenderable( std::move( gl_renderable ) );
                    // Create drawable adapter and link it to the adaptee (user collision)
                    auto gl_drawableAdapter = std::make_unique<TOpenGLDrawableAdapter>( collision->data(), gl_renderableRef );
                    gl_drawableAdapter->SetVisible( false );
                    gl_drawableAdapter->SetWireframe( true );
                    collision->SetDrawable( gl_drawableAdapter.get() );
                    // Keep ownership of the drawable adapter
                    m_vizDrawableAdapters.push_back( std::move( gl_drawableAdapter ) );
                }
            }

            auto visual = singleBody->visual();
            if ( visual )
            {
                auto gl_renderable = loco::glviz::CreateRenderable( visual->data() );
                if ( gl_renderable )
                {
                    // Add engine-renderable to the scene
                    auto gl_renderableRef = m_glApplication->scene()->AddRenderable( std::move( gl_renderable ) );
                    // Create drawable adapter and link it to the adaptee (user visual)
                    auto gl_drawableAdapter = std::make_unique<TOpenGLDrawableAdapter>( visual->data(), gl_renderableRef );
                    visual->SetDrawable( gl_drawableAdapter.get() );
                    // Keep ownership of the drawable adapter
                    m_vizDrawableAdapters.push_back( std::move( gl_drawableAdapter ) );
                }
            }
        }
    }

////     void TOpenGLVisualizer::_CollectCompounds()
////     {
//// 
////     }
//// 
////     void TOpenGLVisualizer::_CollectKintreeAgents()
////     {
//// 
////     }
//// 
////     void TOpenGLVisualizer::_CollectKintreeSensors()
////     {
//// 
////     }
//// 
////     void TOpenGLVisualizer::_CollectTerrainGenerators()
////     {
//// 
////     }

    extern "C" TIVisualizer* visualizer_create( TScenario* scenarioRef,
                                                size_t windowWidth, size_t windowHeight,
                                                bool windowResizable, bool renderOffscreen )
    {
        return new TOpenGLVisualizer( scenarioRef,
                                      windowWidth, windowHeight,
                                      windowResizable, renderOffscreen );
    }
}