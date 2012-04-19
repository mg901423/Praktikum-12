#pragma once
class Vector3
{
		


public:
	
	float x, y, z;

	Vector3(void);
	~Vector3(void);
	Vector3 (float new_x, float new_y, float new_z);
	Vector3 operator+(Vector3 vVector);
	Vector3 operator-(Vector3 vVector) ;
	Vector3 operator*(float number);	
	float skalarni_produkt(Vector3 a, Vector3 b);
	
	float dolzina_vektorja(Vector3 a);
	
	Vector3 operator/(float number);

	
	Vector3 normalizacija(Vector3 a);

	Vector3 vektorski_produkt(Vector3 a, Vector3 b);





};

