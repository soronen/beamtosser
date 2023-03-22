#include "HittableList.h"

bool HittableList::Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const
{
	HitRecord temporaryRecord;
	bool hitAnything = false;
	double closestSoFar = t_max;

	for (const std::shared_ptr<Hittable> object : m_Objects)
	{
		if (object->Hit(r, t_min, closestSoFar, temporaryRecord))
		{
			hitAnything = true;
			closestSoFar = temporaryRecord.t;
			record = temporaryRecord;
		}
	}

	return hitAnything;
}