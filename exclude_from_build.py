Import("env")

def extra_http_configuration(env, node):
    """
    `node.name` - a name of File System Node
    `node.get_path()` - a relative path
    `node.get_abspath()` - an absolute path
    """

    if "LiquidCrystal\\thirdparty libraries" in node.get_path():
        print("Excluding (by exclude_from_build.py) file from build: " + node.get_path())
        return None

    return node

env.AddBuildMiddleware(extra_http_configuration)
