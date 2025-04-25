#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <limits>
#include "main.hpp"

// Global variables
int		options = 0;
char	*program_name = nullptr;

void	open_file(std::ofstream &file, std::string &name)
{
	if (std::ifstream(name))
	{
		std::cout << program_name << ": " << name << ": already exists" << std::endl;
		std::cout << "Do you want to overwrite it? (y/N) ";
		std::string	response;
		std::getline(std::cin, response);
		if (response != "y" && response != "Y")
		{
			std::cout << "Operation cancelled." << std::endl;
			return;
		}
	}
	file.open(name, std::ios::out | std::ios::trunc);
	if (!file.is_open())
		std::cerr << program_name << ": " << name << ": " << strerror(errno) << std::endl;
	else
		std::cout << program_name << ": " << name << ": created" << std::endl;
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

	file << "#ifndef " << macro_name << "\n# define " << macro_name << "\n\nclass "
	<< class_name << "\n{\n\tprivate:\n\t\t// Implementation\n\n\tpublic:\n\t\t"
	<< class_name << "(void);\n\t\t" << class_name << "(const " << class_name
	<< " &copy);\n\t\t~" << class_name << "(void);\n\t\t" << class_name
	<< " &operator = (const " << class_name << " &other);\n};\n\n#endif // "
	<< macro_name << std::endl;
}

void	fill_cpp_file(std::ofstream &file, std::string &filename, char *class_name)
{
	file << "#include \"" << filename.substr(0, filename.length() - 4) << ".hpp\"\n\n"
	<< class_name << "::" << class_name << "(void) {}\n\n" << class_name << "::"
	<< class_name << "(const " << class_name << " &copy)\n{\n\t*this = copy;\n}\n\n"
	<< class_name << "::~" << class_name << "(void) {}\n\n" << class_name << " &"
	<< class_name << "::operator = (const " << class_name << " &other)\n{\n\tif (this != &other)\n"
	"\t{\n\t\t// Implementation\n\t}\n\treturn (*this);\n}" << std::endl;
}

std::string	get_filename(char *class_name, int type)
{
	std::string	filename;

	filename = class_name;
	if ((options & SIMPLE) == 0)
		filename.append(".class");
	if (type == HPP)
		filename.append(".hpp");
	else if (type == CPP)
		filename.append(".cpp");
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

void	create_files(char *argv[])
{
	for (int i = 0 ; argv[i] ; i++)
	{
		if (!check_name_validity(argv[i]))
			std::cerr << program_name << ": " << argv[i] << ": Bad name" << std::endl;
		else
		{
			open_and_fill(argv[i], HPP);
			open_and_fill(argv[i], CPP);
		}
	}

}

void	delete_files(char *argv[])
{
	std::string	filename;

	for (int i = 0 ; argv[i] ; i++)
	{
		if (!check_name_validity(argv[i]))
		{
			std::cerr << program_name << ": " << argv[i] << ": Bad name" << std::endl;
			continue;
		}
		filename = get_filename(argv[i], CPP);
		if (std::remove(filename.c_str()))
			std::cerr << program_name << ": " << filename << ": " << strerror(errno) << std::endl;
		else
			std::cout << program_name << ": " << filename << ": removed" << std::endl;
		filename = get_filename(argv[i], HPP);
		if (std::remove(filename.c_str()))
			std::cerr << program_name << ": " << filename << ": " << strerror(errno) << std::endl;
		else
			std::cout << program_name << ": " << filename << ": removed" << std::endl;
	}
}

int	update_binary(void)
{
	std::cout << "Updating binary..." << std::endl;
	if (system("sh -c \"$(curl -fsSL https://raw.githubusercontent.com/itsmeodx/ClassFileCreator/master/install.sh)\"") == -1)
		if (system("sh -c \"$(wget https://raw.githubusercontent.com/itsmeodx/ClassFileCreator/master/install.sh -0 -)\"") == -1)
			std::cout << "Please install one of the following packages : \n\t- curl\n\t- wget" << std::endl;
	return (EXIT_FAILURE);
}

void	show_usage()
{
	std::cout << program_name << ": try '" << program_name << " -h' for more information." << std::endl;
}
int	show_help(void)
{
	std::cout << "classes' .cpp and .hpp files creator" << std::endl << std::endl;
	std::cout << "usage: " << program_name << " [-options] class_name1 [class_name2 ...]" << std::endl;
	std::cout << "options:" << std::endl;
	std::cout << "    -h: shows this message" << std::endl;
	std::cout << "    -s: create with simple filenames (without '.class')" << std::endl;
	std::cout << "    -d: delete class related files (.cpp and .hpp)" << std::endl;
	std::cout << "        use it with 's' to delete simple files" << std::endl;
	std::cout << "    -u: update the binary" << std::endl;
	return (EXIT_SUCCESS);
}

void	parse_options(char ***argv_ptr)
{
	char	**argv = *argv_ptr;
	int		i = 0;

	while (argv[++i]) {
		if (argv[i][0] != '-')
		{
			break;
		}
		for (int j = 1; argv[i][j]; j++) {
			switch (argv[i][j]) {
				case 'h':
					options |= HELP;
					break;
				case 's':
					options |= SIMPLE;
					break;
				case 'd':
					options |= DELETE;
					break;
				case 'u':
					options |= UPDATE;
					break;
				default:
					std::cerr << "Unknown option: " << argv[i][j] << std::endl;
					options = ERROR;
			}
		}
	}
	(*argv_ptr) += i;
	if (options & ERROR)
		return;
	if (options > UPDATE)
	{
		std::cerr << "Error: -u option cannot be used with other options." << std::endl;
		options = ERROR;
	}
	else if (options > HELP && options < UPDATE)
	{
		std::cerr << "Error: -h option cannot be used with other options." << std::endl;
		options = ERROR;
	}
	else if (argv[i] == nullptr && options <= SIMPLE + DELETE)
	{
		std::cerr << "Error: No class name provided." << std::endl;
		options = ERROR;
	}
}

int	main(int argc, char *argv[])
{
	program_name = argv[0];
	if (argc == 1)
	{
		show_usage();
		return (EXIT_FAILURE);
	}
	parse_options(&argv);
	if (options & ERROR)
		return (EXIT_FAILURE);
	else if (options & UPDATE)
		return (update_binary());
	else if (options & HELP)
		return (show_help());
	else if (options & DELETE)
		delete_files(argv);
	else
		create_files(argv);
	return (EXIT_SUCCESS);
}
