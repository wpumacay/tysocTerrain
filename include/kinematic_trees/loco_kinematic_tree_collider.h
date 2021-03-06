#pragma once

#include <loco_data.h>
#include <kinematic_trees/loco_kinematic_tree_body.h>
#include <kinematic_trees/loco_kinematic_tree_collider_adapter.h>
#include <visualizer/loco_visualizer_drawable_adapter.h>

namespace loco { 
namespace kintree {

    class TKinematicTreeBody;
    class TKinematicTreeColliderAdapter;

    class TKinematicTreeCollider : public TObject
    {
    public :

        TKinematicTreeCollider( const std::string& name,
                                const TCollisionData& collision_data );

        ~TKinematicTreeCollider();

        void SetParentBody( TKinematicTreeBody* parent_body_ref, const TMat4& local_tf );

        void SetColliderAdapter( TIKinematicTreeColliderAdapter* collider_adapter_ref );

        void SetDrawableAdapter( visualizer::TIDrawableAdapter* drawable_adapter_ref );

        void SetVisible( bool visible );

        void SetWireframe( bool wireframe );

        void SetLocalTransform( const TMat4& local_transform );

        void ChangeSize( const TVec3& new_size );

        void ChangeCollisionGroup( int collision_group );

        void ChangeCollisionMask( int collision_mask );

        void ChangeFriction( const TScalar& friction );

        eShapeType shape() const { return m_Data.type; }

        TVec3 size() const { return m_Data.size; }

        int collisionGroup() const { return m_Data.collisionGroup; }

        int collisionMask() const { return m_Data.collisionMask; }

        TScalar friction() const { return m_Data.friction.x(); }

        bool visible() const { return m_Visible; }

        bool wireframe() const { return m_Wireframe; }

        TMat4 local_tf() const { return m_LocalTf; }

        TCollisionData& data() { return m_Data; }

        const TCollisionData& data() const { return m_Data; }

        std::vector<TContactData>& contacts() { return m_Contacts; }

        const std::vector<TContactData>& contacts() const { return m_Contacts; }

        TKinematicTreeBody* parent() { return m_ParentBodyRef; }

        const TKinematicTreeBody* parent() const { return m_ParentBodyRef; }

        TIKinematicTreeColliderAdapter* collider_adapter() { return m_ColliderAdapterRef; }

        const TIKinematicTreeColliderAdapter* collider_adapter() const { return m_ColliderAdapterRef; }

        visualizer::TIDrawableAdapter* drawable_adapter() { return m_DrawableAdapterRef; }

        const visualizer::TIDrawableAdapter* drawable_adapter() const { return m_DrawableAdapterRef; }

        static eObjectType GetStaticType() { return eObjectType::KINEMATIC_TREE_COLLIDER; }

    protected :

        void _InitializeInternal() override;

        void _PreStepInternal() override;

        void _PostStepInternal() override;

        void _ResetInternal() override;

        void _DetachSimInternal() override;

        void _DetachVizInternal() override;

        void _SetTransformInternal( const TMat4& transform ) override;

        eObjectType _GetObjectTypeInternal() const override { return eObjectType::KINEMATIC_TREE_COLLIDER; }

    protected :

        // Construction data of this collider object
        TCollisionData m_Data;
        // Relative transform of collider w.r.t. parent body's frame
        TMat4 m_LocalTf;
        // Reference to the parent-body of this collider object
        TKinematicTreeBody* m_ParentBodyRef;
        // Reference to the drawable resource used for visualization
        visualizer::TIDrawableAdapter* m_DrawableAdapterRef;
        // Reference to the adapter object that gives access to the low-level API for a specific backend
        TIKinematicTreeColliderAdapter* m_ColliderAdapterRef;
        // Flag that indicates the collider's drawable visibility
        bool m_Visible;
        // Flag that indicates the collider's drawable wireframe-mode
        bool m_Wireframe;
        /// List of all currently detected contacts
        std::vector<TContactData> m_Contacts;
    };
}}