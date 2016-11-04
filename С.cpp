#include<iostream>
#include<algorithm>
#include<vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

void readData(size_t & number_of_people, size_t & number_of_buses, int & bus_capacity,
    vector<int> & volume) { // input function
    cin >> number_of_buses >> bus_capacity >> number_of_people;
    volume.resize(number_of_people);
    for (size_t i = 0; i < number_of_people; ++i) {
        cin >> volume[i];
    }
}

int solve(const size_t number_of_people, const size_t number_of_buses, const int bus_capacity,
    const vector<int> & volume) {
    vector< vector< vector <int> > > dp_table(number_of_people); // dp table vector
    for (auto&& person_item : dp_table) {
        person_item.resize(number_of_buses);
        for (auto&& bus_item : person_item) {
            bus_item.resize(static_cast<size_t>(bus_capacity) + 1);
        }
    }

    for (size_t person_index = 0; person_index < number_of_people; ++person_index) {
        for (size_t bus_index = 0; bus_index < number_of_buses; ++bus_index) {
            if (bus_index > 0) { // case, when we go to next bus 
                dp_table[person_index][bus_index][0] =
                    dp_table[person_index][bus_index - 1][bus_capacity];
            }
            for (int capacity_index = 1; capacity_index <= bus_capacity; ++capacity_index) {
                int new_value = 0;
                if (capacity_index >= volume[person_index]) { // case, when we take this person
                    int old_value = 0; 
                    if (person_index > 0) {
                        int new_capacity_index = capacity_index - volume[person_index];
                        old_value = dp_table[person_index - 1][bus_index][new_capacity_index];
                    }
                    new_value = old_value + 1;
                }
                if (person_index > 0) { // case, when we don't take this person
                    new_value = max(
                        new_value,
                        dp_table[person_index - 1][bus_index][capacity_index]
                    );
                }
                if (capacity_index > 0) { // case, when we fill this capacity one
                    new_value = max(
                        new_value,
                        dp_table[person_index][bus_index][capacity_index - 1]
                    );
                }
                dp_table[person_index][bus_index][capacity_index] = new_value;
            }
        }
    }

    return dp_table[number_of_people - 1][number_of_buses - 1][bus_capacity];
}

void printResult(int result) { // output function
    cout << result;
}

int main() {
    size_t number_of_people; // N in statement
    size_t number_of_buses; // M in statement
    int bus_capacity; // D in statement
    vector<int> volume; // volume of each person (L[] in statement)
    
    readData(number_of_people, number_of_buses, bus_capacity, volume);
    int result = solve(number_of_people, number_of_buses, bus_capacity, volume);
    printResult(result);
    
    return 0;
}
