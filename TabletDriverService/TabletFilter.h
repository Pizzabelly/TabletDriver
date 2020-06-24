#pragma once
class TabletFilter {
public:
	virtual void SetTarget(Vector2D vector, double h) = 0;
	virtual void SetPosition(Vector2D vector, double h) = 0;
	virtual bool GetPosition(Vector2D *vector) = 0;
	virtual void Update() = 0;

	void (*callback)();
	float timerInterval;

 	HANDLE thread;

	bool isEnabled;
	bool isValid;

	TabletFilter();

	bool StartTimer();
	bool StopTimer();

};
