#include <stdio.h>
#include <string>
#include <fstream>

namespace aoc
{
std::vector<std::vector<char>> read_antenna_map()
{
    std::string line;
    std::ifstream infile("Input.txt");
    std::vector<std::vector<char>> antenna_map;

    while (std::getline(infile, line)) 
    {
        std::vector<char> row;
        row.reserve(line.size());
        for (char c : line) 
        {
            row.push_back(c);
        }

        antenna_map.push_back(std::move(row));
    }

    return antenna_map;
}

void print_antenna_map(const std::vector<std::vector<char>>& antenna_map)
{
    for (const std::vector<char>& row : antenna_map) 
    {
        for (char c : row) 
        {
            printf("%c ", c);
        }

        printf("\n");
    }
}

int count_anti_nodes(const std::vector<bool>& anti_node_loc_map)
{
    int count = 0;
    for(const bool is_anti_node : anti_node_loc_map)
    {
        if (is_anti_node)
        {
            ++count;        
        } 
    }
    
    return count;
}

bool in_bounds(int row, int num_rows, int col, int num_cols)
{
    return row >= 0 && row < num_rows &&
           col >= 0 && col < num_cols;
}

void populate_anti_nodes_for_freq(
    const std::vector<int>& locations,
    const std::vector<std::vector<char>>& antenna_map,
    const int n_cols,
    const int n_rows,
    std::vector<bool>& out_anti_node_loc_map_p1,
    std::vector<bool>& out_anti_node_loc_map_p2)
{
    for(int i = 0; i < locations.size(); ++i)
    {
        for(int j = i + 1; j < locations.size(); ++j)
        {
            const int first_loc = locations[i];
            const int second_loc = locations[j];

            const int first_row = (first_loc) / n_cols;
            const int first_col = first_loc % (n_cols);

            const int second_row = (second_loc) / n_cols;
            const int second_col = second_loc % (n_cols);

            const int to_second_row = second_row - first_row;
            const int to_second_col = second_col - first_col;

            // Solve p1
            int anti_node_row_one = first_row - to_second_row;
            int anti_node_col_one = first_col - to_second_col;
            if(anti_node_row_one >= 0 && anti_node_row_one < antenna_map.size() &&
                anti_node_col_one >= 0 && anti_node_col_one < n_cols)
            {
                const int first_anti_node_idx = 
                    anti_node_row_one * n_cols + anti_node_col_one;
                out_anti_node_loc_map_p1[first_anti_node_idx] = true;
            }

            int anti_node_row_two = second_row + to_second_row;
            int anti_node_col_two = second_col + to_second_col;
            if(anti_node_row_two >= 0 && anti_node_row_two < antenna_map.size() &&
                anti_node_col_two >= 0 && anti_node_col_two < n_cols)
            {
                const int second_anti_node_idx = 
                    anti_node_row_two * n_cols + anti_node_col_two;
                out_anti_node_loc_map_p1[second_anti_node_idx] = true;
            }     

            // now for p2 
            anti_node_row_one = first_row;
            anti_node_col_one = first_col;

            while(in_bounds(anti_node_row_one, n_rows, anti_node_col_one, n_cols))
            {
                const int first_anti_node_idx = 
                        anti_node_row_one * n_cols + anti_node_col_one;
                    out_anti_node_loc_map_p2[first_anti_node_idx] = true;

                anti_node_row_one -= to_second_row;
                anti_node_col_one -= to_second_col;
            };

            anti_node_row_two = second_row;
            anti_node_col_two = second_col;  

            while(in_bounds(anti_node_row_two, n_rows, anti_node_col_two, n_cols))
            {
                const int second_anti_node_idx = 
                    anti_node_row_two * n_cols + anti_node_col_two;
                out_anti_node_loc_map_p2[second_anti_node_idx] = true;

                anti_node_row_two += to_second_row;
                anti_node_col_two += to_second_col;
            };
        }
    }
}

void populate_anti_nodes(
    const std::vector<std::vector<int>>& freqnency_to_locations, 
    const std::vector<std::vector<char>>& antenna_map,
    const int n_cols,
    const int n_rows,
    std::vector<bool>& out_anti_node_loc_map_p1,
    std::vector<bool>& out_anti_node_loc_map_p2)
{
    for(const auto& locations : freqnency_to_locations)
    {
        populate_anti_nodes_for_freq(locations,
            antenna_map, n_cols, n_rows, 
            out_anti_node_loc_map_p1, out_anti_node_loc_map_p2);
    }
}

void print_antenna_map_with_anti_nodes(const std::vector<std::vector<char>>& antenna_map, 
    std::vector<bool> anti_node_map)
{
    for (int row_idx = 0; row_idx < antenna_map.size(); ++row_idx) 
    {
        const std::vector<char>& row = antenna_map[row_idx];
        for (int col_idx = 0; col_idx < row.size(); ++col_idx) 
        {
            int node_idx = row_idx * row.size() + col_idx;
            if(anti_node_map[node_idx])
            {
                printf("# ");
            }
            else
            {
                printf("%c ", row[col_idx]);
            }
        }

        printf("\n");
    }
}
}

int main()
{
    const std::vector<std::vector<char>> antenna_map = aoc::read_antenna_map();
    const int n_rows = antenna_map.size();
    const int n_cols = antenna_map[0].size();

    auto t = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> freqnency_to_locations(std::pow(2, sizeof(char) * 8));
    for(int row = 0; row < n_rows; ++row)
    {
        for(int col = 0; col < n_cols; ++col)
        {
            const char frequency = antenna_map[row][col];
            if(frequency == '.' || frequency == '\n') 
                continue;
            const int one_dim_idx = row * n_cols + col;
            freqnency_to_locations[frequency].push_back(one_dim_idx);
        }
    }

    const int n_nodes = n_rows * n_cols;
    std::vector<bool> anti_node_loc_map_p1(n_nodes);
    std::vector<bool> anti_node_loc_map_p2(n_nodes);
    aoc::populate_anti_nodes(freqnency_to_locations,
         antenna_map, n_cols, n_rows, anti_node_loc_map_p1, anti_node_loc_map_p2);
    const int anti_node_count_p1 = aoc::count_anti_nodes(anti_node_loc_map_p1);
    const int anti_node_count_p2 = aoc::count_anti_nodes(anti_node_loc_map_p2);

    const unsigned long long duration = 
        (unsigned long long)(std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t).count());

    printf("Executed in: %llu\n", duration);
    printf("Part one answer is: %d\n", anti_node_count_p1);
    printf("Part two answer is: %d\n", anti_node_count_p2);

    return 0;
}