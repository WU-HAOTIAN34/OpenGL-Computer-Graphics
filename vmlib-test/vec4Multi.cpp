#include <catch2/catch_amalgamated.hpp>

#include "../vmlib/mat44.hpp"


TEST_CASE("Vecf4 Multiply", "[mat44]")
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

		Vec4f b = { 1.f, 2.f, 3.f, 4.f };

		auto const identity = a * b;

		REQUIRE_THAT(identity.w, WithinAbs(10.f, kEps_));
		REQUIRE_THAT(identity.x, WithinAbs(10.f, kEps_));
		REQUIRE_THAT(identity.y, WithinAbs(10.f, kEps_));
		REQUIRE_THAT(identity.z, WithinAbs(10.f, kEps_));

		
	}

	SECTION("c * d")
	{

		Mat44f c = { {
			1.f, 2.f, 3.f, 4.f,
			5.f, 6.f, 7.f, 8.f,
			9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f
		} };

		Vec4f d = { 1.f, 1.f, 1.f, 1.f };

		auto const identity = c * d;

		REQUIRE_THAT(identity.w, WithinAbs(58.f, kEps_));
		REQUIRE_THAT(identity.x, WithinAbs(10.f, kEps_));
		REQUIRE_THAT(identity.y, WithinAbs(26.f, kEps_));
		REQUIRE_THAT(identity.z, WithinAbs(42.f, kEps_));


	}


}
