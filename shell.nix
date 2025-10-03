{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "vulkan-dev-hyprland-pure";

  buildInputs = with pkgs; [
    # C++ toolchain
    gcc
    clang
    cmake
    ninja
    pkg-config

    # Vulkan development
    vulkan-headers
    vulkan-loader
    vulkan-tools
    vulkan-validation-layers
    vulkan-utility-libraries
    glslang
    shaderc

    # Wayland-native GLFW
    glfw-wayland

    # Wayland development
    wayland
    wayland-protocols
    libxkbcommon

    # Graphics
    libGL
    glm

    # Development tools
    gdb
    valgrind
    clang-tools

    # Hyprland-specific
    hyprland-protocols
  ];

  shellHook = ''
    echo "=== Pure Wayland Vulkan Development (Hyprland) ==="
    echo "Environment: Wayland-only (No X11 dependencies)"
    echo "Vulkan: $(pkg-config --modversion vulkan 2>/dev/null || echo 'Available')"
    echo "GLFW: Wayland-native"
    echo ""
    
    # Set pure Wayland environment
    export XDG_SESSION_TYPE=wayland
    export GDK_BACKEND=wayland
    export QT_QPA_PLATFORM=wayland
    export CLUTTER_BACKEND=wayland
    export SDL_VIDEODRIVER=wayland
    export MOZ_ENABLE_WAYLAND=1
    
    # Force Wayland for GLFW
    export GLFW_IMPL=wayland
    
    # Vulkan environment
    export VK_LAYER_PATH="${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d"
    
    # Test Vulkan
    if command -v vulkaninfo > /dev/null 2>&1; then
      echo "✓ Vulkan tools available"
      echo "Run 'vkcube --wayland' to test Wayland rendering"
    fi
    
    # Hyprland detection
    if [ "$XDG_CURRENT_DESKTOP" = "Hyprland" ] || [ "$HYPRLAND_INSTANCE_SIGNATURE" ]; then
      echo "✓ Running in Hyprland session"
    else
      echo "⚠ Not in Hyprland session"
    fi
    
    echo ""
    echo "Quick compile:"
    echo "  g++ -std=c++20 main.cpp -o app \$(pkg-config --cflags --libs glfw3 wayland-client vulkan) -ldl"
    echo ""
  '';

  # Environment variables
  VK_LAYER_PATH = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
}
