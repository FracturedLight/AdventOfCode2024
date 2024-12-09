#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

namespace aoc
{
std::vector<char> read_disk()
{
    std::string line;
    std::ifstream infile("Day9.txt");
    std::vector<char> fragmented_disk;

    std::getline(infile, line);
    {
        fragmented_disk.reserve(line.size());
        for (char c : line) 
        {
            fragmented_disk.push_back(c);
        }

    }

    return fragmented_disk;
}

void print_disk(const std::vector<char>& disk)
{
    for(char e : disk)
    {
        printf("%c", e);
    }
}

void print_disk_map(const std::vector<int>& disk_map)
{
    for(int e : disk_map)
    {
        if(e >= 0)
        {
            printf("%d", e);
        }
        else 
        {  
            printf(".");
        }
    }

    printf("\n");
}

std::vector<int> convert_to_disk_map(const std::vector<char>& disk)
{
    std::vector<int> disk_map;
    const int disk_size = disk.size();
    for(int i = 0; i < disk.size(); ++i) 
    {
        const int id = i / 2;
        const char c = disk[i];
        const int digit = c - '0';
        for(int counter = 1; counter <= digit; ++counter)
        {
            if(i % 2 == 0)
            {
                disk_map.push_back(id);
            }
            else 
            {
                disk_map.push_back(-1);
            }
        }
    }

    return disk_map;
}

inline bool space_free(int disk_element)
{
    return disk_element < 0;
}

void compress_disk_map_part_one(std::vector<int>& out_disk_map)
{
    int low = 0;
    int high = out_disk_map.size() - 1;
    while(high > low)
    {
        if(space_free(out_disk_map[low]))
        {
            std::swap(out_disk_map[low], out_disk_map[high]);
            while(space_free(out_disk_map[--high])) {}
        }

        ++low;
    }
}

void compress_disk_map_part_two(std::vector<int>& out_disk_map)
{
    int high = out_disk_map.size() - 1;
    while(high > 0)
    {
        int low = 0;
        // Scan for next file
        while(space_free(out_disk_map[high])) 
        {
            --high;
        }

        const int file = out_disk_map[high];
        int file_counter = 1;
        const int file_end_idx = high;
        // determine file length
        while(out_disk_map[high - 1] == file) 
        {
            --high;
            ++file_counter; 
        }

        const int file_start_idx = file_end_idx - file_counter + 1;

        int free_space_start_idx = -1;
        while(low < high)
        {
            // scan for free space from the bottom
            while(!space_free(out_disk_map[low])) 
            {
                ++low;
            }

            int free_space_counter = 0;
            int current_free_space_start_idx = low;
            while(space_free(out_disk_map[low]) && low < high)
            {
                ++low;
                ++free_space_counter;
            }

            if(free_space_counter >= file_counter)
            {
                free_space_start_idx = current_free_space_start_idx;
                break;
            }
        }

        // Found a place to move the file to
        if(free_space_start_idx != -1)
        {
            // write data over
            for(int i = free_space_start_idx; i < free_space_start_idx + file_counter; ++i)
            {
                out_disk_map[i] = file;
            }

            // free remaining data
            for(int i = file_start_idx; i < file_start_idx + file_counter; ++i)
            {
                out_disk_map[i] = -1;
            }
        }
        else
        {
            --high;
        }
    }
}

long long calculate_check_sum(const std::vector<int>& out_disk_map)
{
    long long check_sum = 0;
    for(int i = 0; i < out_disk_map.size(); ++i)
    {
        if(out_disk_map[i] < 0)
        {
            // can break for p1
            continue;
        }
        
        check_sum += i * out_disk_map[i];
    }

    return check_sum;
}
}

int main()
{
    std::vector<char> disk = aoc::read_disk();

    // pt 1
    {
        std::vector<int> disk_map = aoc::convert_to_disk_map(disk);
        aoc::compress_disk_map_part_one(disk_map);
        const long long check_sum = aoc::calculate_check_sum(disk_map);
        
        printf("part one check sum = %lld\n", check_sum);
    }

    // pt 2
    {
        std::vector<int> disk_map = aoc::convert_to_disk_map(disk);
        aoc::compress_disk_map_part_two(disk_map);
        const long long check_sum = aoc::calculate_check_sum(disk_map);
        
        printf("part two check sum = %lld\n", check_sum);
    }
}