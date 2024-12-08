#include <stdio.h>
#include <string>
#include <fstream>
#include <map>
#include <cassert>

namespace aoc
{
std::vector<std::vector<char>> read_antenna_map()
{
    std::string line;
    std::ifstream infile("Input.txt");
    std::vector<std::vector<char> > antenna_map;

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

void part_one_populate_nodes_by_freq(
    const std::vector<int>& locations,
    const std::vector<std::vector<char>>& antenna_map,
    const int n_cols,
    std::vector<bool>& out_anti_node_loc_map)
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

            const int anti_node_row_one = first_row - to_second_row;
            const int anti_node_col_one = first_col - to_second_col;

            const int anti_node_row_two = second_row + to_second_row;
            const int anti_node_col_two = second_col + to_second_col;  

            const int anti_node_map_size = out_anti_node_loc_map.size();
            if(anti_node_row_one >= 0 && anti_node_row_one < antenna_map.size() &&
                anti_node_col_one >= 0 && anti_node_col_one < n_cols)
            {
                const int first_anti_node_idx = 
                    anti_node_row_one * n_cols + anti_node_col_one;
                out_anti_node_loc_map[first_anti_node_idx] = true;
            }

            if(anti_node_row_two >= 0 && anti_node_row_two < antenna_map.size() &&
                anti_node_col_two >= 0 && anti_node_col_two < n_cols)
            {
                const int second_anti_node_idx = 
                    anti_node_row_two * n_cols + anti_node_col_two;
                out_anti_node_loc_map[second_anti_node_idx] = true;
            }     
        }
    }
}

int part_one_calculate_node_count(
    const std::map<char, std::vector<int>>& freqnency_to_locations, 
    const std::vector<std::vector<char>>& antenna_map,
    const int n_cols,
    std::vector<bool>& out_anti_node_loc_map)
{
    for(auto& [frequency, locations] : freqnency_to_locations)
    {
        part_one_populate_nodes_by_freq(locations,
            antenna_map, n_cols, out_anti_node_loc_map);
    }

    return count_anti_nodes(out_anti_node_loc_map);
}
}

int main()
{
    const std::vector<std::vector<char>> antenna_map = aoc::read_antenna_map();

    auto t = std::chrono::high_resolution_clock::now();
    std::map<char, std::vector<int>> freqnency_to_locations;
    for(int row = 0; row < antenna_map.size(); ++row)
    {
        const int num_cols = antenna_map[row].size();
        for(int col = 0; col < num_cols; ++col)
        {
            const char frequency = antenna_map[row][col];
            if(frequency == '.' || frequency == '\n') 
                continue;
            const int one_dim_idx = row * num_cols + col;
            freqnency_to_locations[frequency].push_back(one_dim_idx);
        }
    }

    std::vector<bool> anti_node_loc_map;
    const int n_cols = antenna_map[0].size();
    anti_node_loc_map.resize(antenna_map.size() * n_cols, false);

    const int node_count_part_one = aoc::part_one_calculate_node_count(freqnency_to_locations,
         antenna_map, n_cols, anti_node_loc_map);
    const unsigned long long part_one_duration = 
        (unsigned long long)(std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - t).count());
    
    printf("Part one executed in: %llu\n", part_one_duration);
    printf("Part one answer is: %d", node_count_part_one);

    return 0;
}