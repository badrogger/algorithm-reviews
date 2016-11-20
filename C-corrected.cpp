#include<iostream>
#include<algorithm>
#include<vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;
using std::ostream;
using std::istream;

// cell in dp table
struct Dp_table_cell {
    size_t person_index;
    size_t bus_index;
    int capacity_index;
};

// input function
void readData(istream & input, size_t & number_of_people, size_t & number_of_buses, 
              int & bus_capacity, vector<int> & volume) {
    input >> number_of_buses >> bus_capacity >> number_of_people;
    volume.resize(number_of_people);
    for (size_t i = 0; i < number_of_people; ++i) {
        input >> volume[i];
    }
}

// function, that initializes dp_table
void initializationDpTable(vector<vector<vector<int>>> & table, const size_t first_size,
                    const size_t second_size, const int third_size) {
    table.resize(first_size);
    for (auto&& person_item : table) {
        person_item.resize(second_size);
        for (auto&& bus_item : person_item) {
            bus_item.resize(static_cast<size_t>(third_size) + 1);
        }
    }
}

// function, that gets dpValue for one cell
int getNewDpValue(const vector<vector<vector<int>>> & dp_table, const Dp_table_cell & cell,
                  const int bus_capacity, const vector<int> & volume) {
    int new_value = 0;
    if (cell.capacity_index > 0) { // case, when we fill current bus
        // if we have capacity for current person
        if (cell.capacity_index >= volume[cell.person_index]) {
            // case, when to get current state we should take this person
            int old_value = 0;
            // if state, that we consider has at least one person
            if (cell.person_index > 0) {
                int new_capacity_index = cell.capacity_index - volume[cell.person_index];
                old_value = dp_table[cell.person_index - 1][cell.bus_index][new_capacity_index];
            }
            new_value = old_value + 1;
        }
        // if state, that we consider has at least one person
        if (cell.person_index > 0) {
            // case, when to get current state we should don't take this person
            new_value = max(
                    new_value,
                    dp_table[cell.person_index - 1][cell.bus_index][cell.capacity_index]
            );
        }
        // if state, that we consider has at least one capacity unit
        if (cell.capacity_index > 0) {
            // case, when to get current state we should mark this place as empty
            new_value = max(
                    new_value,
                    dp_table[cell.person_index][cell.bus_index][cell.capacity_index - 1]
            );
        }
    } else if (cell.bus_index > 0) { // if there are at least one bus
        // case, when to get current state we should go to next bus
        new_value = dp_table[cell.person_index][cell.bus_index - 1][bus_capacity];
    }
    return new_value;

}

// function, that calculates ans
int getMaxPeopleNumber(const size_t number_of_people, const size_t number_of_buses,
                       const int bus_capacity, const vector<int> & volume) {
    vector<vector<vector<int>>> dp_table; // dp table vector
    initializationDpTable(dp_table, number_of_people, number_of_buses, bus_capacity); // dp init
    // dp_table calculation
    for (size_t person_index = 0; person_index < number_of_people; ++person_index) {
        for (size_t bus_index = 0; bus_index < number_of_buses; ++bus_index) {
            for (int capacity_index = 0; capacity_index <= bus_capacity; ++capacity_index) {
                dp_table[person_index][bus_index][capacity_index] =
                        getNewDpValue(dp_table, {person_index, bus_index, capacity_index},
                                      bus_capacity, volume);
            }
        }
    }

    return dp_table[number_of_people - 1][number_of_buses - 1][bus_capacity];
}

// output function
void printResult(ostream & output, int result) { 
    output << result;
}

// main function
int main() {
    size_t number_of_people; // N in statement
    size_t number_of_buses; // M in statement
    int bus_capacity; // D in statement
    vector<int> volume; // volume of each person (L[] in statement)

    readData(cin, number_of_people, number_of_buses, bus_capacity, volume);
    int result = getMaxPeopleNumber(number_of_people, number_of_buses, bus_capacity, volume);
    printResult(cout, result);

    return 0;
}
