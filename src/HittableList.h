#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { Add(object); }

	void Clear() { m_Objects.clear(); }
	void Add(std::shared_ptr<Hittable> object) { m_Objects.push_back(object); }

	bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const
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

private:
	std::vector<std::shared_ptr<Hittable>> m_Objects;
};
