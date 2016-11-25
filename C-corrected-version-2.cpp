#include<iostream>
#include<algorithm>
#include<vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;
using std::ostream;
using std::istream;

typedef vector<vector<vector<int>>> three_d_vector;

// cell in dp table
struct MaxPeopleNumberTableCell {
    int person_index;
    int bus_index;
    int capacity_index;
};

// input function
void readData(istream &input, int &number_of_people, int &number_of_buses,
              int &bus_capacity, vector<int> &volume) {
    input >> number_of_buses >> bus_capacity >> number_of_people;
    volume.resize(static_cast<size_t>(number_of_people));
    for (int i = 0; i < number_of_people; ++i) {
        input >> volume[i];
    }
}

// function, that gets dpValue for one cell
int getNewDpValue(const three_d_vector &max_people_number_table,
                  const MaxPeopleNumberTableCell &cell,
                  const int bus_capacity, const vector<int> &volume) {
    int new_value = 0;
    const int person_index = cell.person_index;
    const int bus_index = cell.bus_index;
    const int capacity_index = cell.capacity_index;
    if (capacity_index > 0) { // case, when we fill current bus
        // if we have capacity for current person
        if (capacity_index >= volume[person_index]) {
            // case, when to get current state we should take this person
            int old_value = 0;
            // if state, that we consider has at least one person
            if (person_index > 0) {
                const int new_capacity = capacity_index - volume[person_index];
                old_value =
                        max_people_number_table[person_index - 1][bus_index][new_capacity];
            }
            new_value = old_value + 1;
        }
        // if state, that we consider has at least one person
        if (person_index > 0) {
            // case, when to get current state we should don't take this person
            new_value = max(
                    new_value,
                    max_people_number_table[person_index - 1][bus_index][capacity_index]
            );
        }
        // if state, that we consider has at least one capacity unit
        if (capacity_index > 0) {
            // case, when to get current state we should mark this place as empty
            new_value = max(
                    new_value,
                    max_people_number_table[person_index][bus_index][capacity_index - 1]
            );
        }
    } else if (bus_index > 0) { // if there are at least one bus
        // case, when to get current state we should go to next bus
        new_value = max_people_number_table[person_index][bus_index - 1][bus_capacity];
    }
    return new_value;

}

// function, that calculates ans
int getMaxPeopleNumber(const int number_of_people, const int number_of_buses,
                       const int bus_capacity, const vector<int> &volume) {
    three_d_vector max_people_number_table; // dp table vector
    //initializing dp table;
    max_people_number_table.resize(static_cast<size_t>(number_of_people));
    for (auto &&person_item : max_people_number_table) {
        person_item.resize(static_cast<size_t>(number_of_buses));
        for (auto &&bus_item : person_item) {
            bus_item.resize(static_cast<size_t>(bus_capacity) + 1);
        }
    }
    // dp_table calculation
    for (int person_index = 0; person_index < number_of_people; ++person_index) {
        for (int bus_index = 0; bus_index < number_of_buses; ++bus_index) {
            for (int capacity_index = 0; capacity_index <= bus_capacity; ++capacity_index) {
                max_people_number_table[person_index][bus_index][capacity_index] =
                        getNewDpValue(
                                max_people_number_table,
                                {person_index, bus_index, capacity_index},
                                bus_capacity, volume
                        );
            }
        }
    }

    return max_people_number_table[number_of_people - 1][number_of_buses - 1][bus_capacity];
}

// output function
void printResult(ostream &output, const int result) {
    output << result;
}

// main function
int main() {
    int number_of_people; // N in statement
    int number_of_buses; // M in statement
    int bus_capacity; // D in statement
    vector<int> volume; // volume of each person (L[] in statement)

    readData(cin, number_of_people, number_of_buses, bus_capacity, volume);
    const int result = getMaxPeopleNumber(number_of_people, number_of_buses, bus_capacity, volume);
    printResult(cout, result);

    return 0;
}

