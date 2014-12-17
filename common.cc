// Config
bool DEBUG = true;
std::string JUDGE_HOME = "/usr/local/judge_runner";
const char * LANG_C[] = {"gcc", "main.c", "-o", "main", NULL};

// Global variable
std::string working_dir;

// Problem data
int time_limit = 1000;  // ms
int memory_limit = 128;  // MB
std::string input_path;

// Solution data
int solution_id = 0;
std::string solution_path;
int lang = 0;

void _log(std::string s) {
    if (DEBUG) {
        std::cout << s << std::endl;
    }
}

bool _dir_exist(const std::string &path) {
    DIR *dp;
    if ((dp = opendir(path.c_str())) == NULL) return false;
    closedir(dp);
    return true;
}

void _copy_file(const std::string &source, const std::string &target) {
    system(("cp " + source + " " + target).c_str());
}

size_t _get_filesize(std::string filename) {
    struct stat st;
    if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;   
}