#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "BaseCamera.hpp"

class TrackballCamera : public BaseCamera
{
private:
	float _distance;
	float _theta;		// Rotation around Z or X axis
	float _phi;			// Rotation around Y axis

	bool _CanTurn;    // Is Able to Turn ?
	float _lastX;     // Last Mouse coordinates
	float _lastY;
	float _sensitivity;

public: 
	TrackballCamera()
		: _distance(-5.0f), _theta(0.0f), _phi(0.0f), _CanTurn(false),
		  _lastX(450.0f), _lastY(320.0f), _sensitivity(8.0f) {}

	void moveFront(float delta) override
	{ 
		if((_distance < -2 && delta > 0) || (_distance > -14 && delta < 0))
			_distance += delta; 
		std::cout << _distance << std::endl;
	}
	void rotateLeft(float angle) override { _phi += glm::radians(angle); }
	void rotateUp(float angle) override { _theta += glm::radians(angle); }
	
	glm::mat4 getViewMatrix() const override
	{
		glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, _distance));
		ViewMatrix = glm::rotate(ViewMatrix, glm::radians(_theta), glm::vec3(1, 0, 0));
		ViewMatrix = glm::rotate(ViewMatrix, glm::radians(_phi), glm::vec3(0, 1, 0));

		return ViewMatrix;
	}

	// Getters
	bool GetCanTurn() const override { return _CanTurn; }
	float GetLastX() const override { return _lastX; }
	float GetLastY() const override { return _lastY; }
	float GetSensitivity() const override { return _sensitivity; }

	// Setters
	void SetCanTurn(bool condition) override { _CanTurn = condition; }
	void SetLastX(float x) override { _lastX = x; }
	void SetLastY(float y) override { _lastY = y; }
};