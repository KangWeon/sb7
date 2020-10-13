#ifndef __ARCBALL_H__
#define __ARCBALL_H__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;

using glm::perspective;
using glm::lookAt;
//using glm::frustum;

using glm::identity;
using glm::translate;
using glm::rotate;
//using glm::scale;

using glm::radians;
using glm::value_ptr;

namespace sb7
{

    namespace utils
    {

        class arcball
        {
        public:
            arcball()
                : normalizedDims(-1.0f, 1.0f),
                width(1.0f),
                height(1.0f),
                dragging(false)
            {

            }

            void reset(void)
            {
                mouseStart = vec3(0.0f, 0.0f, 0.0f);
                mouseCurrent = vec3(0.0f, 0.0f, 0.0f);

                lastRotation = quat(0.0f, 0.0f, 0.0f, 1.0f);
                currentRotation = quat(0.0f, 0.0f, 0.0f, 1.0f);

                rotationMatrix = glm::toMat4(currentRotation);
                
                lastRotationMatrix = identity<mat4>();
            }

            void setDimensions(float w, float h)
            {
                width = w;
                height = h;

                normalizedDims[0] = 1.0f / w;
                normalizedDims[1] = 1.0f / h;
            }

            void onMouseDown(float x, float y)
            {
                vec2 mouseClip(2.0f * x * normalizedDims[0] - 1.0f, 1.0f - 2.0f * y * normalizedDims[1]);

                pointToSphere(mouseClip, mouseStart);
                lastRotation = currentRotation;
                lastRotationMatrix = rotationMatrix;

                dragging = true;
            }

            void onMouseUp(void)
            {
                dragging = false;
            }

            void onMouseMove(float x, float y)
            {
                if (dragging)
                {
                    vec2 mouseClip(2.0f * x * normalizedDims[0] - 1.0f, 1.0f - 2.0f * y * normalizedDims[1]);

                    pointToSphere(mouseClip, mouseCurrent);

                    vec3 perpVector = normalize(cross(mouseStart, mouseCurrent));

                    if (dot(perpVector, perpVector) > 0.00001f)
                    {
                        // quat q = vec4(perpVector, cosf(acosf(dot(mouseStart, mouseCurrent)) * 0.5f));
                        // quat q = vec4(2.0f * 3.14159267f * cosf(x * 0.01f), vec3(0.0f, 0.0f, 1.0f)); // cosf(acosf(dot(mouseStart, mouseCurrent)) * 0.5f));

                        // q = normalize(q);
                        // currentRotation = q * lastRotation;

                        // quatToMatrix(q, rotationMatrix);
                        rotationMatrix = rotate(acosf(dot(mouseStart, mouseCurrent)) * 200.0f, -perpVector) * lastRotationMatrix;
                    }
                    else
                    {
                        dragging = true;
                    }
                }
            }

            const quat getRotation() const
            {
                return currentRotation;
            }

            const mat4 getRotationMatrix() const
            {
                return rotationMatrix;
            }

        private:
            float               width;
            float               height;

            float               start_x;
            float               start_y;
            bool                dragging;

            vec2         normalizedDims;

            quat                lastRotation;
            quat                currentRotation;
            vec3                mouseStart;
            vec3                mouseCurrent;
            mat4                lastRotationMatrix;
            mat4                rotationMatrix;

            void pointToSphere(const vec2& pt, vec3& vec);
        };

        void arcball::pointToSphere(const vec2& pt, vec3& vec)
        {
            float length2 = dot(pt, pt);

            if (length2 <= 1.0f)
            {
                vec = vec3(pt, sqrt(1.0f - length2));
            }
            else
            {
                float norm = 1.0f / sqrt(length2);
                vec = vec3(pt * norm, 0.0f);
            }
        }

    }

}

#endif /* __ARCBALL_H__ */
