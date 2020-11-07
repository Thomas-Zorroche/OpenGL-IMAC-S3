#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

class TrackballCamera
{
private:
	float _distance;
	float _theta;
	float _phi;
	bool _CanTurn;    // Is Able to Turn ?
	float _lastX;     // Last Mouse coordinates
	float _lastY;
	float _sensitivity;

public: 
	TrackballCamera()
		: _distance(-5.0f), _theta(0.0f), _phi(0.0f), _CanTurn(false),
		  _lastX(450.0f), _lastY(320.0f), _sensitivity(8.0f) {}

	void moveFront(float delta) 
	{ 
		if((_distance < -2 && delta > 0) || (_distance > -14 && delta < 0))
			_distance += delta; 
		std::cout << _distance << std::endl;
	}
	void rotateLeft(float angle) { _phi += glm::radians(angle); }
	void rotateUp(float angle) { _theta += glm::radians(angle); }
	
	void SetCanTurn(bool condition) { _CanTurn = condition; }
	bool GetCanTurn() const { return _CanTurn; }
	
	void SetLastX(float x) { _lastX = x; }
	void SetLastY(float y) { _lastY = y; }
	float GetLastX() const { return _lastX; }
	float GetLastY() const { return _lastY; }

	float GetSensitivity() const { return _sensitivity; }

	glm::mat4 getViewMatrix() const
	{
		glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, _distance));
		ViewMatrix = glm::rotate(ViewMatrix, glm::radians(_theta), glm::vec3(0, 1, 0));
		ViewMatrix = glm::rotate(ViewMatrix, glm::radians(_phi), glm::vec3(1, 0, 0));

		return ViewMatrix;
	}
};