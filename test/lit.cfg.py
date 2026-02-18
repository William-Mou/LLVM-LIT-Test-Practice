import os
import lit.formats

config.name = 'JSMinifier Test'
config.test_format = lit.formats.ShTest(True)
config.suffixes = ['.js']

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.my_obj_root, "test")

config.substitutions.append(('%jsm', os.path.join(config.my_obj_root, 'js-minifier')))

# setup PATH for FileCheck
_llvm_bin = os.environ.get('LLVM_BIN')
if _llvm_bin is None:
    _llvm_bin = os.path.join(
        os.environ.get('LLVM_BUILD_DIR', os.path.expanduser('~/llvm/llvm-project/build')),
        'bin',
    )
_path = config.environment.get('PATH', os.environ.get('PATH', ''))
config.environment['PATH'] = os.pathsep.join([_llvm_bin, _path])
