#pragma once

#include <components/collisions.h>

namespace tysoc {

    class TCollision;

    class TICollisionAdapter
    {
        public :

        TICollisionAdapter( TCollision* collisionPtr ) { m_collisionPtr = collisionPtr; }

        virtual ~TICollisionAdapter() {}

        virtual void build() = 0;

        virtual void reset() = 0;

        virtual void update() = 0;

        virtual void setLocalPosition( const TVec3& position ) = 0;

        virtual void setLocalRotation( const TMat3& rotation ) = 0;

        virtual void setLocalTransform( const TMat4& transform ) = 0;

        virtual void changeSize( const TVec3& newSize ) = 0;

        TCollision* collision() { return m_collisionPtr; }

        protected :

        TCollision* m_collisionPtr;
    };

    /* dl-function to be loaded from the backend support */
    typedef TICollisionAdapter* FcnCreateCollisionAdapter( TCollision* collisionPtr );

}