#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace ogl {

	class Camera {
	public:

		Camera();

		/*
		The position of the camera.
		*/
		const glm::vec3& position() const;

		void setPosition(const glm::vec3& position);

		void offsetPosition(const glm::vec3& offset);

		/*
		The vertical viewing angle of the camera, in degrees.

		Determines how "wide" the view of the camera is. Large angles appear to be zoomed out, as the camera has a wide view. Small values appear to be zoomed in, as the camera has a very narrow view.

		The value must be between 0 and 180.
		*/
		float fieldOfView() const;

		void setFieldOfView(float fieldOfView);

		/*
		The closest visible distance from the camera.

		Objects that are closer to the camera than the near plane distance will not be visible.

		Value must be greater than 0.
		*/
		float nearPlane() const;

		/*
		The farthest visible distance from the camera.

		Objects that are further away from the than the far plane distance will not be visible.

		Value must be greater than the near plane
		*/
		float farPlane() const;

		/*
		Sets the near and far plane distances.

		Everything between the near plane and the far plane will be visible. Everything closer than the near plane, or farther than the far plane, will not be visible.

		@param nearPlane Minimumvisible distance from camera. Must be > 0
		@param farPlane Maximum visible distance from camera. Must be > nearPlane
		*/
		void setNearAndFarPlane(float nearPlane, float farPlane);

		/*
		A rotation matrix that determines the direction the camera is looking.

		Does not inlcude translation (the camera's position).
		*/
		glm::mat4 orientation() const;

		/*
		Offsets the cameras orientation.

		The verticle angle is constrained between 85deg and -85deg to avoid gimbal lock.

		@param upAngle the angle (in degrees) to offset upwards. Negative values are downwards.
		@param rightAngle the angle (in degrees) to offset rightwards. Negative values are leftwards.
		*/
		void offsetOrientation(float upAngle, float rightAngle);

		/*
		Orients the camera so that it is directly facing 'position'

		@param position the position to look at
		*/
		void lookAt(glm::vec3 position);

		/*
		The width divided by the height of the screen/window/viewport

		Incorrect values will make the 3D scene look stretched.
		*/
		float viewportAspectRatio() const;
		void setViewportAspectRatio(float viewportAspectRatio);

		/*
		A unit vector representing the directin the camera is facing
		*/
		glm::vec3 forward() const;

		/*
		A unit vector representing the firection to the right  of the camera
		*/
		glm::vec3 right() const;

		/*
		A unit vector representing the direction out of the top of the camera
		*/
		glm::vec3 up() const;

		/*
		The combined camera transformation matrix, including perspective projection.

		This is the complete matrix to use in the vertex shader.
		*/
		glm::mat4 matrix() const;

		/*
		The perspective projection transformation matrix
		*/
		glm::mat4 projection() const;

		/*
		The translation and rotation matrix of the camera.

		Same as the 'matrix' method, except the return value does not include the projection transformation
		*/
		glm::mat4 view() const;

	private:
		glm::vec3 _position;
		float _horizontalAngle;
		float _verticalAngle;
		float _fieldOfView;
		float _nearPlane;
		float _farPlane;
		float _viewportAspectRatio;

		void normalizeAngles();
	};

}