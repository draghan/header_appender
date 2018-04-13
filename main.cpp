/*
    This file is distributed under MIT License.

    Copyright (c) 2018 draghan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <vector>
#include <string>
#include <experimental/filesystem>
#include <fstream>
#include <streambuf>
#include <iostream>

namespace fs = std::experimental::filesystem;

bool string_contains(const std::string &source, const std::string &what);

std::vector<fs::path> get_files_from_directory(const std::string &directory_path,
                                               const std::vector<std::string> &extensions_whitelist = {},
                                               const std::vector<std::string> &directories_blacklist = {});

bool append_header(const fs::path &filepath, const std::string &header);

int main()
{
    const std::string license_header{R"(/*
    This file is distributed under MIT License.

    Copyright (c) 2018 draghan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

)"};

    const std::string project_path{"C:/Users/draghan/Desktop/trashcan/projekty/bt"};
    const std::vector<std::string> files_to_change{".cpp", ".hpp"};
    const std::vector<std::string> excluded_directories{"cmake", "external"};

    const auto files = get_files_from_directory(project_path, files_to_change, excluded_directories);

    for(const auto &file : files)
    {
        if(!append_header(file, license_header))
        {
            std::cout << "!! ";
        }
        std::cout << file.string() << '\n';
    }

    std::cout << "\n" << files.size() << '\n';
}


bool string_contains(const std::string &source, const std::string &what)
{
    return source.find(what) != std::string::npos;
}


std::vector<fs::path> get_files_from_directory(const std::string &directory_path,
                                               const std::vector<std::string> &extensions_whitelist,
                                               const std::vector<std::string> &directories_blacklist)
{
    auto is_directory_on_blacklist = [&directories_blacklist](const fs::path &directory)
    {
        return std::find_if(directories_blacklist.begin(),
                            directories_blacklist.end(),
                            [&](const auto &directory_from_blacklist)
                            {
                                return string_contains(directory.string(), directory_from_blacklist);
                            }
        ) != directories_blacklist.end();
    };

    auto file_has_proper_extension = [&extensions_whitelist](const fs::path &file)
    {
        return std::find(extensions_whitelist.begin(),
                         extensions_whitelist.end(),
                         file.extension()) != extensions_whitelist.end();
    };

    std::vector<fs::path> content;

    for(auto &file : fs::recursive_directory_iterator(directory_path))
    {
        if(!fs::is_directory(file) &&
           !is_directory_on_blacklist(file.path().parent_path()) &&
           file_has_proper_extension(file))
        {
            content.push_back(file);
        }
    }
    return content;
}

bool append_header(const fs::path &filepath, const std::string &header)
{
    std::fstream file{filepath.string(), std::ios::in};
    if(!file.good())
    {
        return false;
    }
    std::string file_content{std::istreambuf_iterator<char>(file),
                             std::istreambuf_iterator<char>()};
    file.close();

    file.open(filepath.string(), std::ios::out);
    if(!file.good())
    {
        return false;
    }
    file << header + file_content;
    return true;
}
