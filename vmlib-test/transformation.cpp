#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"


TEST_CASE("transformation", "[mat44]")
{
	static constexpr float kEps_ = 1e-6f;

	using namespace Catch::Matchers;

	SECTION("0.")
	{
		auto const identity = make_translation({0.f, 0.f, 0.f});

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

	SECTION("1 2 3")
	{
		auto const right = make_translation({ 1.f, 2.f, 3.f });

		REQUIRE_THAT(right(0, 0), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(0, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(0, 3), WithinAbs(1.f, kEps_));

		REQUIRE_THAT(right(1, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 1), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(1, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(1, 3), WithinAbs(2.f, kEps_));

		REQUIRE_THAT(right(2, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(2, 2), WithinAbs(1.f, kEps_));
		REQUIRE_THAT(right(2, 3), WithinAbs(3.f, kEps_));

		REQUIRE_THAT(right(3, 0), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 1), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 2), WithinAbs(0.f, kEps_));
		REQUIRE_THAT(right(3, 3), WithinAbs(1.f, kEps_));
	}

	
}
