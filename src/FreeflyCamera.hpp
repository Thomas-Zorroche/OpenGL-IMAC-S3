#pragma once

//#define _USE_MATH_DEFINES
#define M_PI 3.14
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "BaseCamera.hpp"

class FreeflyCamera : public BaseCamera
{
private:
	glm::vec3 _Position;	  // Position of the camera
	float _phi;
	float _theta;
	
	glm::vec3 _FrontVector;   // Direction
	glm::vec3 _LeftVector;    // Left
	glm::vec3 _UpVector;      // Up

	bool _CanTurn;    // Is Able to Turn ?
	float _lastX;     // Last Mouse coordinates
	float _lastY;
	float _sensitivity;

public:
	FreeflyCamera()
		: _Position(0, 0, -5), _phi(M_PI), _theta(0), _CanTurn(false),
		_lastX(450.0f), _lastY(320.0f), _sensitivity(8.0f) 
	{
		computeDirectionVectors();
	}

	~FreeflyCamera() {}

private:
	void computeDirectionVectors()
	{
		// Direction
		_FrontVector = glm::vec3(glm::cos(glm::radians(_theta)) * glm::sin(glm::radians(_phi)),
								 glm::sin(glm::radians(_theta)),
								 glm::cos(glm::radians(_theta)) * glm::cos(glm::radians(_phi)));

		// Left
		_LeftVector = glm::vec3((glm::sin(glm::radians(_phi) + (M_PI / 2))),
								 0,
								 glm::cos(glm::radians(_phi) + (M_PI / 2)));
		// Up
		_UpVector = glm::cross(_FrontVector, _LeftVector);
	}

public:
	void moveFront(float t)	override 
	{ 
		_Position += t * _FrontVector; 
		computeDirectionVectors();
	}
	void moveLeft(float t) override 
	{ 
		_Position += t * _LeftVector; 
		computeDirectionVectors();
	}

	void rotateUp(float angle) override 
	{ 
		_theta -= glm::radians(angle); 
		computeDirectionVectors();
	}
	void rotateLeft(float angle) override 
	{ 
		_phi -= glm::radians(angle); 
		computeDirectionVectors();
	}

	glm::mat4 getViewMatrix() const override
	{
		return glm::lookAt(_Position, _Position + _FrontVector, _UpVector);
	}

	// Getters
	bool GetCanTurn() const override { return _CanTurn; }
	float GetLastX() const override { return _lastX; }
	float GetLastY() const override { return _lastY; }
	float GetSensitivity() const override { return _sensitivity; }
	virtual char GetActiveKey() const override { return _ActiveKey; };
	virtual float GetSpeed() const override { return _Speed; };


	// Setters
	void SetCanTurn(bool condition) override { _CanTurn = condition; }
	void SetLastX(float x) override { _lastX = x; }
	void SetLastY(float y) override { _lastY = y; }
	virtual void SetActiveKey(char key) override { _ActiveKey = key; };

};
