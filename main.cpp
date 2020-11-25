//File manager

#include <iostream>
#include <ios>
#include <iomanip>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

fs::path cur = fs::current_path();

void help () //documentation
{

}

void pwd()
{
    std::cout << std::setw(40) << std::setfill('-') << '-' << std::endl;

    if (cur.string().size() > 38)
    {
        std::string new_str = cur.string().substr(cur.string().size() - 38, 38);
        std::cout  << '|' << std::setw(38) << std::setfill(' ') 
            << new_str << '|' << std::endl;

    } else {

        std::cout << '|' << std::setw(38) << std::setfill(' ') 
            << cur.string() << '|' << std::endl;

    }

    std::cout << std::setw(40) << std::setfill('-') << '-' << std::endl;
}

void ls ()
{
    std::cout << std::setw(40) << std::setfill('-') << '-' << std::endl;

    std::string str;
    for (auto &p : fs::directory_iterator(cur))
    {
        fs::path pth (p);
        std::cout << '|' << std::setw(38)  << std::setfill(' ') 
                   << pth.filename().string() << '|' << std::endl;
    }

    std::cout << std::setw(40) << std::setfill('-') << '-' << std::endl;
}

void stat (std::string& cmd) 
{
    int pos = cmd.find(' ', 0); //get rid of space in the end
    cmd = cmd.substr(0, pos);

    fs::path p = cur/cmd;
    if (fs::exists(p) && !(fs::is_directory(p)))
    {
        std::cout << std::setw(40) << std::setfill('-') << '-' << std::endl;
        std::cout << '|' << " (in bytes)" << std::setfill(' ') << std::setw(27) 
                  << fs::file_size(p) << '|' << std::endl;
        std::cout << std::setw(40) << std::setfill('-') << '-' << std::endl;
    } else std::cout << "Unable to get info about a file.\nMaybe it`s a directory or it doesn't exist.\nYou'd better check your spelling)\n";
}

void mkdir (std::string& cmd)
{
    int pos = cmd.find(' ', 0); //get rid of space in the end
    cmd = cmd.substr(0, pos);

    fs::path p = cur/cmd;
    if (!(fs::exists(p)))
        fs::create_directories(p);
    else std::cout << "This directory(ies) already exist\n";
}

void cd (std::string& cmd)
{
    int pos = cmd.find(' ', 0); //get rid of space in the end
    cmd = cmd.substr(0, pos);

    if (cmd == "..")
    {
        cur = cur.remove_filename();
        if (cur.string().size() > 1)
            cur = cur.string().substr(0, cur.string().size() - 1);
    } else {
        if (fs::exists(cur/cmd) && (fs::is_directory(cur/cmd)))
            cur = cur/cmd;
        else std::cout << cmd << " doesn't exist or it's not a directory\n";
    }
}

void rm (std::string& cmd)
{
    int pos = cmd.rfind(' ', cmd.size());
    if (pos == cmd[cmd.size() - 1])
        cmd = cmd.substr(0, pos);

    if (cmd.find("-rf") != -1 || cmd.find("-fr") != -1) //removing of a directory recursively
    {
        int pos2 = cmd.rfind(' ', cmd.size());
        cmd = cmd.substr(pos2 + 1, cmd.size() - pos2 - 1);

        if (fs::exists(cur/cmd) && (fs::is_directory(cur/cmd)))
            fs::remove_all(cur/cmd);
        else std::cout << cmd << " doesn't exist or it's not a directory\n";
    } else {                                            //removing of a file 
        int pos2 = cmd.rfind(' ', cmd.size());
        cmd = cmd.substr(pos2 + 1, cmd.size() - pos2 - 1);

        if (fs::exists(cur/cmd) && (fs::is_regular_file(cur/cmd)))
            fs::remove(cur/cmd);
        else std::cout << cmd << " doesn't exist or it's not a file\n";
    }
}

void cp (std::string& cmd)
{
    int pos = cmd.rfind(' ', cmd.size());
    if (pos == cmd.size() - 1)
        cmd = cmd.substr(0, pos);
    
    int pos2 = cmd.rfind(' ');
    std::string dest = cmd.substr(pos2 + 1, cmd.size() - pos2 - 1);

    pos = dest.rfind('/');
    if (dest.size() - 1 == pos) 
        dest = dest.substr(0, dest.size() - 1);

    cmd = cmd.substr(0, pos2);

    fs::path p1 = cur/cmd;
    fs::path p2 = static_cast<fs::path>(dest)/cmd;

    if (fs::exists(cur/cmd) && (fs::is_regular_file(cur/cmd)))
        fs::copy_file(p1, p2);
    else std::cout << cmd << " doesn't exist or it's not a directory\n";
}

void rename (std::string& cmd)
{
    int pos = cmd.rfind(' ', cmd.size());
    if (pos == cmd.size() - 1)
        cmd = cmd.substr(0, pos);

    int pos2 = cmd.rfind(' ');
    std::string name2 = cmd.substr(pos2 + 1, cmd.size() - pos2 - 1);

    cmd = cmd.substr(0, pos2);

    if(fs::exists(cur/cmd))
        fs::rename(cur/cmd, cur/name2);
    else std::cout << "Unable to rename " << cmd;
}

void mv (std::string& cmd)
{
    int pos = cmd.rfind(' ', cmd.size());
    if (pos == cmd.size() - 1)
        cmd = cmd.substr(0, pos);

    int pos2 = cmd.rfind(' ');
    std::string dest = cmd.substr(pos2 + 1, cmd.size() - pos2 - 1);

    pos = dest.rfind('/');
    if (dest.size() - 1 == pos) 
        dest = dest.substr(0, dest.size() - 1);

    cmd = cmd.substr(0, pos2);

    if(fs::exists(cur/cmd))
        fs::rename(cur/cmd, static_cast<fs::path>(dest)/cmd);
    else std::cout << "Unable to move file/directory\n";
}

int main ()
{
    help();
    std::string cmd, base_cmd, other_cmd;

    std::cout << "It's your turn now)"<< std::endl;

    do {
        printf("\033[0;96mCommand: \033[0m"); //ANSI escape sequences for coloring
        getline(std::cin, cmd);

        int space = cmd.find(' ', 0);
        base_cmd = cmd.substr(0, space);
        other_cmd = cmd.substr(space + 1, cmd.size() - space -1);

        if (base_cmd == "pwd")
            pwd();
        else if (base_cmd == "ls")
            ls();
        else if (base_cmd == "stat")
        {
            if (cmd > base_cmd)
                stat(other_cmd);
            else std::cout << "stat: missing operand\n";
        } else if (base_cmd == "mkdir")
        {
            if (cmd > base_cmd)
                mkdir(other_cmd);
            else std::cout << "mkdir: missing operand\n";
        } else if (base_cmd == "cd")
        {
            if (cmd > base_cmd)
                cd(other_cmd);
            else std::cout << "cd: missing operand\n";
        } else if (base_cmd == "rm")
        {
            if (cmd > base_cmd)
                rm(other_cmd);
            else std::cout << "rm: missing operand\n";
        } else if (base_cmd == "cp")
        {
            if (cmd > base_cmd)
                cp(other_cmd);
            else std::cout << "cp: missing operand\n";
        } else if (base_cmd == "rename")
        {
            if (cmd > base_cmd)
                rename(other_cmd);
            else std::cout << "rename: missing operand\n";
        } else if (base_cmd == "mv")
        {
            if (cmd > base_cmd)
                mv(other_cmd);
            else std::cout << "mv: missing operand\n";
        }
        
    } while (cmd != "quit" && cmd != "q");

    return 0;
}
