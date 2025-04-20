#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include "main.hpp"

int s;

void	open_file(std::ofstream &file, std::string &name)
{
	file.open(name, std::ios::out | std::ios::app);
	if (!file.is_open())
	{
		std::cerr << "Error : opening file : " << name << '.' << std::endl;
		std::exit(1);
	}
}

std::string	get_macro_name(std::string &filename)
{
	size_t		length;
	std::string	macro_name;

	length = filename.length();
	for (size_t i = 0 ; i < length ; i++)
	{
		if (filename[i] == '.')
			macro_name.push_back('_');
		else
			macro_name.push_back(std::toupper(filename[i]));
	}
	return (macro_name);
}

void	fill_hpp_file(std::ofstream &file, std::string &name, char *class_name)
{
	std::string	macro_name = get_macro_name(name);

	file << "#ifndef " << macro_name << std::endl;
	file << "# define " << macro_name << std::endl << std::endl;
	file << "class " << class_name << std::endl << '{' << std::endl << "\tpublic :" << std::endl;
	file << "\t\t" << class_name << "(void);" << std::endl;
	file << "\t\t" << class_name << '(' << class_name << " const &copy);" << std::endl;
	file << "\t\t" << '~' << class_name << "(void);" << std::endl;
	file << "\t\t" << class_name << " const &operator = (" << class_name << " const &other);" << std::endl;
	file << "};" << std::endl;
	file << std::endl << "#endif";
}

void	fill_cpp_file(std::ofstream &file, std::string &filename, char *class_name)
{
	file << "#include \"" << filename.substr(0, filename.length() - 4) << ".hpp\"" << std::endl << std::endl;
	file << class_name << "::" << class_name << "(void) {}" << std::endl << std::endl;
	file << class_name << "::" << class_name << '(' << class_name << " const &copy)" << std::endl << "{" << std::endl << "\t*this = copy;" << std::endl << "}" << std::endl << std::endl;
	file << class_name << "::~" << class_name << "(void) {}" << std::endl << std::endl;
	file << class_name << " const\t&" << class_name << "::operator = (" << class_name << " const &other)" << std::endl << "{" << std::endl << "\treturn (*this);" << std::endl << "}";
}

std::string	get_filename(char *class_name, int type)
{
	std::string	filename;

	filename = class_name;
	if (type == HPP)
	{
		if (s)
			filename.append(".hpp");
		else
			filename.append(".class.hpp");
	}
	if (type == CPP)
	{
		if (s)
			filename.append(".cpp");
		else
			filename.append(".class.cpp");
	}
	return (filename);
}

void	open_and_fill(char *class_name, int type)
{
	std::string		filename;
	std::ofstream	file;

	filename = get_filename(class_name, type);
	open_file(file, filename);
	if (type == HPP)
		fill_hpp_file(file, filename, class_name);
	else if (type == CPP)
		fill_cpp_file(file, filename, class_name);
	file.close();
}

void	show_help(char *program_name)
{
	std::cout << "classes' .cpp and .hpp files creator" << std::endl << std::endl;
	std::cout << "usage: " << program_name << " [-options] class_name1 [class_name2 ...]" << std::endl;
	std::cout << "options:" << std::endl;
	std::cout << "    -h: shows this message" << std::endl;
	std::cout << "    -s: create with simple filenames (without '.class')" << std::endl;
	std::cout << "    -d: delete class related files (.cpp and .hpp)" << std::endl;
	std::cout << "        use it with -s to delete simple files" << std::endl;
	std::cout << "    -u: update the binary" << std::endl;
	std::exit(0);
}

bool	check_name_validity(char *class_name)
{
	int	length;

	length = std::strlen(class_name);
	if (!isalpha(class_name[0]) && (class_name[0] != '_'))
		return (0);
	for (int i = 1 ; i < length ; i++)
		if (!std::isalnum(class_name[i]) && (class_name[i] != '_'))
			return (0);
	return (1);
}

void	delete_files(char *argv[])
{
	std::string	filename;

	for (int i = 2 ; argv[i] ; i++)
	{
		filename = get_filename(argv[i], CPP);
		if (std::remove(filename.c_str()))
			std::cerr << "Error removing file : " << filename << std::endl;
		filename = get_filename(argv[i], HPP);
		if (std::remove(filename.c_str()))
			std::cerr << "Error removing file : " << filename << std::endl;
	}
}

int	get_options(char *argv1)
{
	int			result = 0;
	std::string	options = argv1;

	if (argv1[0] != '-')
		return (result);
	if (options.find_first_of('h') != std::string::npos)
		result |= HELP;
	if (options.find_first_of('s') != std::string::npos)
		result |= SIMPLE;
	if (options.find_first_of('d') != std::string::npos)
		result |= DELETE;
	if (options.find_first_of('u') != std::string::npos)
		result |= UPDATE;
	return (result);
}

void	create_files(char *argv[])
{
	for (int i = s + 1 ; argv[i] ; i++)
	{
		if (!check_name_validity(argv[i]))
			std::cerr << "Error : " << argv[i] << " ; Bad name" << std::endl;
		else
		{
			open_and_fill(argv[i], HPP);
			open_and_fill(argv[i], CPP);
		}
	}

}

int	update_binary()
{
	std::cout << "Updating binary..." << std::endl;
	if (system("sh -c \"$(curl -fsSL https://raw.githubusercontent.com/itsmeodx/ClassFileCreator/master/install.sh)\"") == -1)
		if (system("sh -c \"$(wget https://raw.githubusercontent.com/itsmeodx/ClassFileCreator/master/install.sh -0 -)\"") == -1)
			std::cout << "Please install one of the following packages : \n\t- curl\n\t- wget" << std::endl;
	exit(EXIT_FAILURE);
}

int	main(int argc, char *argv[])
{
	int	result;

	if (argc > 1)
	{
		result = get_options(argv[1]);
		if (result & UPDATE)
			return (update_binary());
		if (result & HELP)
			show_help(argv[0]);
		if (result & SIMPLE)
			s = 1;
		if (result & DELETE)
			delete_files(argv);
		else
			create_files(argv);
	}
	else
	{
		std::cout << argv[0] << ": try '"
			<< argv[0] << " -h' for more information." << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
