import os

# vcvarsall.bat зовёт vswhere.exe по короткому имени. Если его нет в PATH,
# ругань уходит в stdout и ломает UTF-16 разбор вывода -> setuptools решает,
# что Visual Studio не установлена. Добавляем каталог vswhere в PATH.
_installer = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer"
if os.path.isdir(_installer):
    os.environ["PATH"] = _installer + os.pathsep + os.environ["PATH"]

from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

ext = Pybind11Extension(
    "crowdsim._core",
    ["cpp/bindings/module.cpp"],
    include_dirs=["cpp/include"],
    cxx_std=20,
)

setup(
    name="crowdsim",
    version="0.1.0",
    packages=["crowdsim"],
    package_dir={"": "python"},
    ext_modules=[ext],
    cmdclass={"build_ext": build_ext},
)
