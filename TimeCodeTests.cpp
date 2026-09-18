#include "TimeCode.h"
#include <iostream>
#include <cassert>

using namespace std;

void TestComponentsToSeconds(){
	cout << "Testing ComponentsToSeconds" << endl;
	
	// random but "safe" inputs
	long long unsigned int t = TimeCode::ComponentsToSeconds(3, 17, 42);
	assert(t == 11862);

	// all zeros
	assert(TimeCode::ComponentsToSeconds(0, 0, 0) == 0);

	// Top of a single hour.
	assert(TimeCode::ComponentsToSeconds(0, 59, 59) == 3599);

	// roll-over
	assert(TimeCode::ComponentsToSeconds(0, 0, 90) == 90);

	cout << "PASSED!" << endl << endl;
}


void TestDefaultConstructor(){
	cout << "Testing Default Constructor" << endl;
	TimeCode tc;
	
	//cout << "Testing ToString()" << endl;
	//cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	cout << "PASSED!" << endl << endl;
}


void TestComponentConstructor(){
	cout << "Testing Component Constructor" << endl;
	TimeCode tc = TimeCode(0, 0, 0);
	//cout << "Testing ToString()" << endl;
	//cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	// more tests go here!
	
	// Roll-over inputs
	TimeCode tc3 = TimeCode(3, 71, 3801);
	//cout << "tc3: " << tc3.ToString() << endl;
	assert(tc3.ToString() == "5:14:21");
	
	// More tests go here!
	
	cout << "PASSED!" << endl << endl;
}


void TestGetComponents(){
	cout << "Testing GetComponents" << endl;
	
	unsigned int h;
	unsigned int m;
	unsigned int s;
	
	// Regular values
	TimeCode tc = TimeCode(5, 2, 18);
	tc.GetComponents(h, m, s);
	assert(h == 5 && m == 2 && s == 18);
	
	// More tests go here!
	
	cout << "PASSED!" << endl << endl;
}


void TestSubtract(){
	cout << "Testing Subtract" << endl;
	TimeCode tc1 = TimeCode(1, 0, 0);
	TimeCode tc2 = TimeCode(0, 50, 0);
	TimeCode tc3 = tc1 - tc2;
	assert(tc3.ToString() == "0:10:0");


	TimeCode tc4 = TimeCode(1, 15, 45);
	try{
		TimeCode tc5 = tc1 - tc4;
		cout << "tc5: " << tc5.ToString() << endl;
		assert(false);
	}
	catch(const invalid_argument& e){
		// just leave this empty
		// and keep doing more tests
	}

	// subtracting an equal value should not throw
	TimeCode tc6 = tc1 - tc1;
	assert(tc6.ToString() == "0:0:0");

	cout << "PASSED!" << endl << endl;
}


void TestSetSeconds()
{
	cout << "Testing SetSeconds" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetSeconds(45); // test valid change
	assert(tc.ToString() == "8:5:45");

	try
	{
		tc.SetSeconds(60);  // test invalid change
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		cout << e.what() << endl;
	}

	assert(tc.ToString() == "8:5:45");

	cout << "PASSED!" << endl << endl;
}


void TestSetHours()
{
	cout << "Testing SetHours" << endl;

	TimeCode tc = TimeCode(1, 2, 3);
	tc.SetHours(10);
	assert(tc.ToString() == "10:2:3");

	// hours are allowed to grow arbitrarily large.
	tc.SetHours(1000000);
	assert(tc.GetHours() == 1000000);

	cout << "PASSED!" << endl << endl;
}


void TestReset()
{
	cout << "Testing reset" << endl;

	TimeCode tc = TimeCode(3, 4, 5);
	tc.reset();
	assert(tc.ToString() == "0:0:0");

	cout << "PASSED!" << endl << endl;
}


void TestGetTimeCodeAsSeconds()
{
	cout << "Testing GetTimeCodeAsSeconds" << endl;

	TimeCode tc = TimeCode(1, 2, 3);
	assert(tc.GetTimeCodeAsSeconds() == TimeCode::ComponentsToSeconds(1, 2, 3));

	cout << "PASSED!" << endl << endl;
}


void TestCopyConstructor()
{
	cout << "Testing Copy Constructor" << endl;

	TimeCode original = TimeCode(1, 2, 3);
	TimeCode copy = TimeCode(original);
	assert(copy == original);

	// editiung the copy should not affect the original.
	// they should be independent, not share a reference.
	copy.SetHours(9);
	assert(copy.GetHours() == 9);
	assert(original.GetHours() == 1);

	cout << "PASSED!" << endl << endl;
}


void TestOperatorPlus()
{
	cout << "Testing operator+" << endl;

	TimeCode tc1 = TimeCode(0, 50, 0);
	TimeCode tc2 = TimeCode(0, 20, 0);
	TimeCode tc3 = tc1 + tc2;
	assert(tc3.ToString() == "1:10:0"); // minutes roll over into hours

	// operands must not be mutated by the operator.
	assert(tc1.ToString() == "0:50:0");
	assert(tc2.ToString() == "0:20:0");

	cout << "PASSED!" << endl << endl;
}


void TestOperatorMultiply()
{
	cout << "Testing operator*" << endl;

	TimeCode tc = TimeCode(1, 0, 0); // 3600 seconds
	TimeCode doubled = tc * 2.0;
	assert(doubled.ToString() == "2:0:0");

	TimeCode zeroed = tc * 0.0;
	assert(zeroed.ToString() == "0:0:0");

	// Fractional results are truncated, not rounded.
	TimeCode tc2 = TimeCode(0, 0, 3601);
	TimeCode half = tc2 * 0.5;
	assert(half.GetTimeCodeAsSeconds() == 1800);

	try
	{
		TimeCode negative = tc * -1.0;
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// expected
	}

	cout << "PASSED!" << endl << endl;
}


void TestOperatorDivide()
{
	cout << "Testing operator/" << endl;

	TimeCode tc = TimeCode(2, 0, 0); // 7200 seconds
	TimeCode halved = tc / 2;
	assert(halved.ToString() == "1:0:0");

	// dividing by exactly 1 leaves the value unchanged.
	TimeCode same = tc / 1;
	assert(same == tc);

	// fractional divisors below 1 are allowed as long as they're positive.
	TimeCode doubled = tc / 0.5;
	assert(doubled.ToString() == "4:0:0");

	try
	{
		TimeCode result = tc / 0;
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// expected
	}

	try
	{
		TimeCode result = tc / -2;
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// expected
	}

	cout << "PASSED!" << endl << endl;
}


void TestComparisonOperators()
{
	cout << "Testing comparison operators" << endl;

	TimeCode a = TimeCode(1, 0, 0);
	TimeCode b = TimeCode(1, 0, 0);
	TimeCode c = TimeCode(2, 0, 0);

	assert(a == b);
	assert(!(a == c));

	assert(a != c);
	assert(!(a != b));

	assert(a < c);
	assert(!(c < a));

	assert(a <= b);
	assert(a <= c);
	assert(!(c <= a));

	assert(c > a);
	assert(!(a > c));

	assert(a >= b);
	assert(c >= a);
	assert(!(a >= c));

	cout << "PASSED!" << endl << endl;
}


void TestSetMinutes()
{
	cout << "Testing SetMinutes" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetMinutes(15); // test valid change
	assert(tc.ToString() == "8:15:9");

	try
	{
		tc.SetMinutes(80);  // test invalid change
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// cout << e.what() << endl;
	}

	assert(tc.ToString() == "8:15:9");

	cout << "PASSED!" << endl << endl;
}


// Many More Tests...

	
int main(){
	
	TestComponentsToSeconds();
	TestDefaultConstructor();
	TestComponentConstructor();
	TestGetComponents();
	TestSubtract();
	TestSetMinutes();
	TestSetSeconds();
	TestSetHours();
	TestReset();
	TestGetTimeCodeAsSeconds();
	TestCopyConstructor();
	TestOperatorPlus();
	TestOperatorMultiply();
	TestOperatorDivide();
	TestComparisonOperators();

	cout << "PASSED ALL TESTS!!!" << endl;
	return 0;
}
