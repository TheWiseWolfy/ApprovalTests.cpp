#define APPROVALS_DOCTEST
#include "ApprovalTests.hpp"

#include "DocTestReporter.h"

#include <filesystem>
namespace fs = std::filesystem;

using namespace ApprovalTests;

//auto defaultReporterDisposer =
//    Approvals::useAsDefaultReporter(std::make_shared<TextDiffReporter>());

//auto defaultReporterDisposer =
//    Approvals::useAsDefaultReporter(std::make_shared<DocTestReporter>());

auto configChange = ApprovalTestNamer::setCheckBuildConfig(false);

bool isRunningInBuildEnvironment()
{
    ApprovalTestNamer namer;
    auto path1 = fs::canonical(fs::path(".")).string();
    std::cout << "--- ApprovalTestNamer::getTestSourceDirectory(): "
              << namer.getTestSourceDirectory() << '\n';
    std::cout << "--- ApprovalTestNamer::getDirectory(): " << namer.getDirectory()
              << '\n';
    std::cout << "--- Current working directory: " << path1 << '\n';

    std::string ending = "examples/out_of_source";
    ending = StringUtils::replaceAll(ending, "/", SystemUtils::getDirectorySeparator());
    return StringUtils::endsWith(path1, ending);
}

auto default_namer_disposer = Approvals::useAsDefaultNamer([]() {
    std::string args =
        "{TestFileName}.{TestCaseName}.{ApprovedOrReceived}.{FileExtension}";
    bool is_build_environment = isRunningInBuildEnvironment();

    if (is_build_environment)
    {
        args = "{TestSourceDirectory}/" + args;
    }
    else if (!SystemUtils::safeGetEnv("APPROVED_FILES_ROOT_DIR").empty())
    {
        args = SystemUtils::safeGetEnv("APPROVED_FILES_ROOT_DIR") + "/" + args;
    }
    return std::make_shared<TemplatedCustomNamer>(args);
});
