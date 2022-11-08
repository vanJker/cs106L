#include <iostream>
#include <fstream>
#include <unordered_set>
#include <algorithm>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;


// some useful functions
using std::ifstream;
string fileToString(ifstream& file);
unordered_set<string> findWikiLinks(const string& page_html);

int main() {
	
    /* TODO: Write code here! */

    /* Note if your file reading isn't working, please go to the
     * projects tab on the panel on the left, and in the run section,
     * uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */

    cout << "Enter a valid filename:" << endl;
    string filename;
    std::cin >> filename;
    ifstream ifs(filename);

    // print valid wiki links
    string page_html = fileToString(ifs);
    unordered_set<string> links = findWikiLinks(page_html);
    for (string link : links) {
        cout << link << endl;
    }

    // diff with output file
    ifstream output_ifs("[output]-" + filename);
    unordered_set<string> output_links;
    string output_link;
    while (output_ifs >> output_link) {
        output_links.insert(output_link);
    }

    if (links.size() != output_links.size()) {
        cout << "\n----FAILURE----\n" << endl;
        return 0;
    }
    for (string link : links) {
        if (output_links.find(link) == output_links.end()) {
            cout << "\n----FAILURE-----\n" << endl;
            return 0;
        }
    }
    cout << "\n----PASSED----\n" << endl;

    return 0;
}

string fileToString(ifstream& file) {
    string ret = "";
    string line;
    while (std::getline(file, line)) {
        ret += line + " ";
    }
    return ret;
}

unordered_set<string> findWikiLinks(const string& page_html) {
    unordered_set<string> links;
    string toFind = "href=\"/wiki/";
    auto curr = page_html.begin();
    auto end = page_html.end();

    while (curr != end) {
        auto found = std::search(curr, end, toFind.begin(), toFind.end());
        if (found == end) break;

        // get valid wiki link
        auto link_begin = found + toFind.length();
        auto link_end = link_begin;
        while (*link_end != '\"') {
            ++link_end;
        }

        // check contain any disallowed char (# or :)
        auto isContainDisallow = [](char iter) {
            return iter != '#' && iter != ':';
        };
        if (std::all_of(link_begin, link_end, isContainDisallow)) {
            links.insert(string(link_begin, link_end));
        }

        curr = found + 1;
    }

    return links;
}














