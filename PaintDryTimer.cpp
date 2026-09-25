#include <ctime> // for time(0)
#include <iostream> // for cin and cout
#include <cmath> // for M_PI and others
#include <vector> // for vectors (duh)
#include <cstdlib> // for random
#include <cassert> // for assert in the tests() function
#include "TimeCode.h" // for timecode's (duh)
#include <cctype> // for tolower()
#include <string>

using namespace std;

struct DryingSnapShot {
	string name;
	time_t startTime;
	TimeCode *timeToDry;
};

long long int get_time_remaining(DryingSnapShot dss){
	time_t currentTime = time(0);

  long long int elapsed = static_cast<long long int>(currentTime - dss.startTime);

  long long int dryTime = static_cast<long long int>(dss.timeToDry->GetTimeCodeAsSeconds());
	
  return dryTime - elapsed;
}

string drying_snap_shot_to_string(DryingSnapShot dss){
	long long int remaining = get_time_remaining(dss);

  	string output = dss.name + " (takes " + dss.timeToDry->ToString() + " to dry) ";

	if (remaining <= 0) {
		output += "Done";
	} else {
		TimeCode remainingTime(0, 0, remaining);

		output += "time remaining; " + remainingTime.ToString();
	}
	return output;
}

double get_sphere_sa(double rad){
	return 4 * M_PI * rad * rad;
}

TimeCode *compute_time_code(double surfaceArea){
	unsigned long long int seconds = static_cast<unsigned long long int>(surfaceArea);

	return new TimeCode(0, 0, seconds);
}

void tests(){
	// get_time_remaining
    DryingSnapShot dss;
    dss.startTime = time(0);
    TimeCode tc = TimeCode(0, 0, 7);
    dss.timeToDry = &tc;
    long long int ans = get_time_remaining(dss);
    assert(ans > 6 && ans < 8);

    // test that elapsed time is subtracted correctly.
    DryingSnapShot dss2;
    dss2.startTime = time(0) - 3;
    TimeCode tcElapsed = TimeCode(0, 0, 10);
    dss2.timeToDry = &tcElapsed;

    long long int ans2 = get_time_remaining(dss2);
    assert(ans2 >= 6 && ans2 <= 7);

    // test a finished batch.
    DryingSnapShot dss3;
    dss3.startTime = time(0) - 10;
    TimeCode tcDone = TimeCode(0, 0, 5);
    dss3.timeToDry = &tcDone;

    assert(get_time_remaining(dss3) <= 0);


    // get_sphere_sa
    double sa = get_sphere_sa(2.0);
    assert(50.2654 < sa && sa < 50.2655);

    // rad 0 should have surface area 0.
    assert(get_sphere_sa(0.0) == 0.0);

    // rad 1 should have surface area about 12.566.
    double sa2 = get_sphere_sa(1.0);
    assert(12.5663 < sa2 && sa2 < 12.5664);


    // compute_time_code
    TimeCode *tc2 = compute_time_code(1.0);
    assert(tc2->GetTimeCodeAsSeconds() == 1);
    delete tc2;

    // fractional surface area should be truncated to whole seconds
    TimeCode *fractional = compute_time_code(12.9);
    assert(fractional->GetTimeCodeAsSeconds() == 12);
    delete fractional;

    // 0 surface area should produce 0 drying time.
    TimeCode *zeroTime = compute_time_code(0.0);
    assert(zeroTime->GetTimeCodeAsSeconds() == 0);
    delete zeroTime;


    // Radius 2 should have surface area about 50.265,
    // which becomes 50 seconds after truncating
    TimeCode* tc3 = compute_time_code(get_sphere_sa(2.0));

    assert(tc3->GetTimeCodeAsSeconds() == 50);

    delete tc3;

    // Test snapshot string
    DryingSnapShot testSnapshot;

    testSnapshot.name = "Test-Batch";
    testSnapshot.startTime = time(0);
    testSnapshot.timeToDry = new TimeCode(0, 0, 10);

    string text = drying_snap_shot_to_string(testSnapshot);

    assert(text.find("Test-Batch") != string::npos);
    assert(text.find("time remaining") != string::npos);

    delete testSnapshot.timeToDry;

	cout << "ALL TESTS PASSED!" << endl;
}


int main(){
	tests();

	vector<DryingSnapShot> batches;

	char option;

	while(true) {
		cout << "Choose an option: (A)dd, (V)iew Current Items, (Q)uit: ";

		cin >> option;

		// Add new batch
		if (tolower(option) == 'a') {
			double radius;

			cout << "radius: ";
			cin>>radius;

			if (radius<0){
				cout << "radius cannot be negative" << endl;
				continue;
			}

			double surfaceArea = get_sphere_sa(radius);

			DryingSnapShot newBatch;

			newBatch.name = "Batch-" + to_string(rand());

			newBatch.startTime = time(0);

			// Heap allocation
			newBatch.timeToDry = compute_time_code(surfaceArea);

			batches.push_back(newBatch);

			cout << drying_snap_shot_to_string(newBatch) << endl;
		} else if (tolower(option) == 'v') {
			// erase shifts loop to next element
			for (size_t i = 0; i < batches.size();) {
				cout << drying_snap_shot_to_string(batches[i]) << endl;

				// if the batch is done, free timecode before removing it from the vector
				if (get_time_remaining(batches[i]) <= 0) {
					delete batches[i].timeToDry;

					batches.erase(batches.begin() + i);
				} else {
					++i;
				}
			}

			cout << batches.size() << " batches being tracked" << endl;
		} else if (tolower(option) == 'q') {
			// Some batches could still be drying when user quits
			// so their heap memory has to be freed
			for (DryingSnapShot& batch : batches) {
				delete batch.timeToDry;
			}

			batches.clear();

			break;
		} else {
			cout << "invalid option" << endl;
		}
	}

	return 0;
}
