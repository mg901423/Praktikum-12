#include "Vector3.h"
#include <math.h>

	Vector3::Vector3(void)
	{
	}


	Vector3::~Vector3(void)
	{
	}

	Vector3::Vector3 (float new_x, float new_y, float new_z) 
	{
		x = new_x; y = new_y; z = new_z;
	}
	
	Vector3 Vector3::operator+(Vector3 vVector)
	{
		return Vector3(vVector.x+x, vVector.y+y, vVector.z+z);
	}
	
	Vector3 Vector3::operator-(Vector3 vVector) 
	{
		return Vector3(x-vVector.x, y-vVector.y, z-vVector.z);
	}
	
	Vector3 Vector3::operator*(float number)	
	{
		return Vector3(x*number, y*number, z*number);
	}
	
	Vector3 Vector3::operator/(float number)
	{
		return Vector3(x/number, y/number, z/number);
	}
	
	Vector3 Vector3::normalizacija(Vector3 a)
	{
		float d=((a.x*a.x) + (a.y*a.y) + (a.z*a.z));
		float dol = sqrt(d);
		return Vector3(a.x * (1 /dol) , a.y * (1/dol) , a.z * (1/dol));
	}
	Vector3 Vector3::vektorski_produkt(Vector3 a, Vector3 b)
	{
		return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	}
		
	float Vector3::skalarni_produkt(Vector3 a, Vector3 b)
	{
		return a.x * b.x + a.y + b.y + a.z + b.z;
	}
	float Vector3::dolzina_vektorja(Vector3 a)
	{
		float d=((a.x*a.x) + (a.y*a.y) + (a.z*a.z));
		return sqrt(d);
	}
