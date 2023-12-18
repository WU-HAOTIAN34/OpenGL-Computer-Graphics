#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"

// See mat44-rotation.cpp first.

TEST_CASE("Perspective projection", "[mat44]")
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	SECTION("Standard")
	{
		auto const proj = make_perspective_projection(
			60.f * 3.1415926f / 180.f,
			1280 / float(720),
			0.1f, 100.f
		);

		REQUIRE_THAT(proj(0, 0), WithinAbs(0.974279, kEps_));
		REQUIRE_THAT(proj(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(proj(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(1, 1), WithinAbs(1.732051f, kEps_));
		REQUIRE_THAT(proj(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(proj(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(2, 2), WithinAbs(-1.002002f, kEps_));
		REQUIRE_THAT(proj(2, 3), WithinAbs(-0.200200f, kEps_));

		REQUIRE_THAT(proj(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(proj(3, 2), WithinAbs(-1.f, kEps_));
		REQUIRE_THAT(proj(3, 3), WithinAbs(0.f, kEps_));
	}
}