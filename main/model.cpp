#include "model.hpp"

#include "../vmlib/mat33.hpp"


SimpleMeshData make_cylinder(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform)
{
	std::vector<Vec3f> pos;
	std::vector<Vec3f> norms;
	float prevY = std::cos(0.f);
	float prevZ = std::sin(0.f);

	Mat33f N = mat44_to_mat33(transpose(invert(aPreTransform)));

	for (std::size_t i = 0; i <= aSubdivs; ++i) {
		float const angle = i / float(aSubdivs) * 2.f * 3.1415926f;
		float y = std::cos(angle);
		float z = std::sin(angle);

		Vec3f normal = Vec3f{ 0, y, z }; 
		Vec3f transformedNormal = N * normal;
		
		pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
		norms.emplace_back(transformedNormal);

		pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });
		norms.emplace_back(transformedNormal);

		pos.emplace_back(Vec3f{ 1.f, y, z });
		norms.emplace_back(transformedNormal);

		pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
		norms.emplace_back(transformedNormal);

		pos.emplace_back(Vec3f{ 1.f, y, z });
		norms.emplace_back(transformedNormal);

		pos.emplace_back(Vec3f{ 0.f, y, z });
		norms.emplace_back(transformedNormal);

		prevY = y;
		prevZ = z;
	}

	if (aCapped) {
	
		for (std::size_t i = 0; i < aSubdivs; ++i) {
			float const angle = (i + 1) / float(aSubdivs) * 2.f * 3.1415926f;
			float y = std::cos(angle);
			float z = std::sin(angle);
			Vec3f normal = Vec3f{ -1, 0, 0 };
			Vec3f transformedNormal = N * normal;
			
			pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });
			norms.emplace_back(transformedNormal);
			pos.emplace_back(Vec3f{ 0.f, prevY, prevZ });
			norms.emplace_back(transformedNormal);
			pos.emplace_back(Vec3f{ 0.f, y, z });
			norms.emplace_back(transformedNormal);

			
			normal = Vec3f{ 1, 0, 0 }; 
			transformedNormal = N * normal;
			pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
			norms.emplace_back(transformedNormal);

			pos.emplace_back(Vec3f{ 1.f, y, z });
			norms.emplace_back(transformedNormal);

			pos.emplace_back(Vec3f{ 1.f, prevY, prevZ });
			norms.emplace_back(transformedNormal);


			prevY = y;
			prevZ = z;
		}
	}
	for (auto& p : pos) {
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;
		p = Vec3f{ t.x, t.y, t.z };
	}

	std::vector col(pos.size(), aColor);

	return SimpleMeshData{ std::move(pos), std::move(col), std::move(norms) };
}

Vec3f cross(const Vec3f& v1, const Vec3f& v2) {
	return Vec3f{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

SimpleMeshData make_cone(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
	std::vector<Vec3f> pos;
	std::vector<Vec3f> norms;

	Mat33f N = mat44_to_mat33(transpose(invert(aPreTransform)));

	for (std::size_t i = 0; i < aSubdivs; ++i) {
		float const angle = (i + 1) / float(aSubdivs) * 2.f * 3.1415926f;
		float y = std::cos(angle);
		float z = std::sin(angle);
		float prevY = std::cos(i / float(aSubdivs) * 2.f * 3.1415926f);
		float prevZ = std::sin(i / float(aSubdivs) * 2.f * 3.1415926f);

		Vec3f tip = Vec3f{ 0.f, 1.f, 0.f }; 
		Vec3f base1 = Vec3f{ 1.f, 0.f, prevZ }; 
		Vec3f base2 = Vec3f{ 1.f, 0.f, z }; 

		pos.emplace_back(tip);
		pos.emplace_back(base1);
		pos.emplace_back(base2);

		Vec3f edge1 = base1 - tip;
		Vec3f edge2 = base2 - tip;
		Vec3f normal = normalize(cross(edge2 , edge1)); 
		Vec3f transformedNormal = normalize(N * normal); 

		norms.emplace_back(transformedNormal);
		norms.emplace_back(transformedNormal);
		norms.emplace_back(transformedNormal);
	}

	if (aCapped) {
		
		Vec3f capNormal = { 0.0f, -1.f, 0.f };
		Vec3f transformedCapNormal = normalize(N * capNormal); 

		for (std::size_t i = 0; i < aSubdivs; ++i) {
			float const angle = (i + 1) / float(aSubdivs) * 2.f * 3.1415926f;
			float y = std::cos(angle);
			float z = std::sin(angle);
			float prevY = std::cos(i / float(aSubdivs) * 2.f * 3.1415926f);
			float prevZ = std::sin(i / float(aSubdivs) * 2.f * 3.1415926f);

			
			pos.emplace_back(Vec3f{ 1.f, 0.f, 0.f });
			pos.emplace_back(Vec3f{ 1.f, prevY, prevZ }); 
			pos.emplace_back(Vec3f{ 1.f, y, z }); 

			
			norms.emplace_back(transformedCapNormal);
			norms.emplace_back(transformedCapNormal);
			norms.emplace_back(transformedCapNormal);
		}
	}

	for (auto& p : pos) {
		Vec4f p4{ p.x, p.y, p.z, 1.f };
		Vec4f t = aPreTransform * p4;
		t /= t.w;
		p = Vec3f{ t.x, t.y, t.z };
	}

	
	for (auto& n : norms) {
		Vec4f n4{ n.x, n.y, n.z, 0.f }; 
		n = normalize(N * Vec3f{ n4.x, n4.y, n4.z }); 
	}

	std::vector col(pos.size(), aColor);

	return SimpleMeshData{ std::move(pos), std::move(col), std::move(norms) };
}

