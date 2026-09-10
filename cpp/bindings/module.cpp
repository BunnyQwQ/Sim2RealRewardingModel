#include <pybind11/pybind11.h>

namespace py = pybind11;

// Имя модуля должно посимвольно совпадать с именем цели в
// pybind11_add_module(), иначе интерпретатор не найдёт PyInit__core.
PYBIND11_MODULE(_core, m) {
    m.doc() = "crowdsim C++ core (private extension module)";
    m.attr("__version__") = "0.1.0";

    m.def(
        "add",
        [](int a, int b) { return a + b; },
        py::arg("a"),
        py::arg("b"),
        "Smoke test: returns a + b."
    );
}
