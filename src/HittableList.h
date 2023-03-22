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

	virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override;

private:
	std::vector<std::shared_ptr<Hittable>> m_Objects;
};
