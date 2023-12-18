#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"


TEST_CASE("4x4 rotation", "[mat44]")
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	SECTION("0.-z")
	{
		auto const identity = make_rotation_z(0.f);

		REQUIRE_THAT(identity(0, 0), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(1, 1), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(2, 2), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("60 degrees-z")
	{
		auto const right = make_rotation_z(3.1415926f / 3.f);

		REQUIRE_THAT(right(0, 0), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(0, 1), WithinAbs(- pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(1, 0), WithinAbs(pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(1, 1), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 2), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("-60 degrees-z")
	{
		auto const right = make_rotation_z(-3.1415926f / 3.f);

		REQUIRE_THAT(right(0, 0), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(0, 1), WithinAbs(pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(1, 0), WithinAbs(-pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(1, 1), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 2), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("0.-x")
	{
		auto const identity = make_rotation_x(0.f);

		REQUIRE_THAT(identity(0, 0), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(1, 1), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(2, 2), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("60 degrees-x")
	{
		auto const right = make_rotation_x(3.1415926f / 3.f);

		REQUIRE_THAT(right(0, 0), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 1), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(1, 2), WithinAbs(-pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 1), WithinAbs(pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(2, 2), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("-60 degrees-x")
	{
		auto const right = make_rotation_x(-3.1415926f / 3.f);

		REQUIRE_THAT(right(0, 0), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 1), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(1, 2), WithinAbs(pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 1), WithinAbs(-pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(2, 2), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("0.-y")
	{
		auto const identity = make_rotation_y(0.f);

		REQUIRE_THAT(identity(0, 0), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(1, 1), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(2, 2), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(identity(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(identity(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(identity(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("60 degrees-y")
	{
		auto const right = make_rotation_y(3.1415926f / 3.f);

		REQUIRE_THAT(right(0, 0), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 2), WithinAbs(pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 1), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(2, 0), WithinAbs(-pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 2), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 3), WithinAbs(1.f, kEps_));
	}

	SECTION("-60 degrees-y")
	{
		auto const right = make_rotation_y(-3.1415926f / 3.f);

		REQUIRE_THAT(right(0, 0), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 2), WithinAbs(-pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(0, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 1), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(2, 0), WithinAbs(pow(3, 0.5) / 2.f, kEps_));
		REQUIRE_THAT(right(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 2), WithinAbs(0.5f, kEps_));
		REQUIRE_THAT(right(2, 3), WithinAbs(0.f, kEps_));

		REQUIRE_THAT(right(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 3), WithinAbs(1.f, kEps_));
	}
}
