#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"


TEST_CASE("Mat44 Multiply", "[mat44]")
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	SECTION("a * b")
	{
		

		Mat44f a = { {
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f
		} };

		Mat44f b = { {
			1.f, 2.f, 3.f, 4.f,
			5.f, 6.f, 7.f, 8.f,
			9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f
		} };

		auto const identity = a * b;

		REQUIRE_THAT(identity(0, 0), WithinAbs(28.f, kEps_));
		REQUIRE_THAT(identity(0, 1), WithinAbs(32.f, kEps_));
		REQUIRE_THAT(identity(0, 2), WithinAbs(36.f, kEps_));
		REQUIRE_THAT(identity(0, 3), WithinAbs(40.f, kEps_));

		REQUIRE_THAT(identity(1, 0), WithinAbs(28.f, kEps_));
		REQUIRE_THAT(identity(1, 1), WithinAbs(32.f, kEps_));
		REQUIRE_THAT(identity(1, 2), WithinAbs(36.f, kEps_));
		REQUIRE_THAT(identity(1, 3), WithinAbs(40.f, kEps_));

		REQUIRE_THAT(identity(2, 0), WithinAbs(28.f, kEps_));
		REQUIRE_THAT(identity(2, 1), WithinAbs(32.f, kEps_));
		REQUIRE_THAT(identity(2, 2), WithinAbs(36.f, kEps_));
		REQUIRE_THAT(identity(2, 3), WithinAbs(40.f, kEps_));

		REQUIRE_THAT(identity(3, 0), WithinAbs(28.f, kEps_));
		REQUIRE_THAT(identity(3, 1), WithinAbs(32.f, kEps_));
		REQUIRE_THAT(identity(3, 2), WithinAbs(36.f, kEps_));
		REQUIRE_THAT(identity(3, 3), WithinAbs(40.f, kEps_));
	}

	


}
