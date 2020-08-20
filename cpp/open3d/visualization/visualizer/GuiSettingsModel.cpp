// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "open3d/visualization/visualizer/GuiSettingsModel.h"

#include "open3d/utility/Console.h"

namespace open3d {
namespace visualization {

const std::vector<GuiSettingsModel::LightingProfile>
        GuiSettingsModel::lighting_profiles_ = {
                {.name = "Bright day with sun at +Y [default]",
                 .ibl_intensity = 45000,
                 .sun_intensity = 45000,
                 .sun_dir = {0.577f, -0.577f, -0.577f}},
                {.name = "Bright day with sun at -Y",
                 .ibl_intensity = 45000,
                 .sun_intensity = 45000,
                 .sun_dir = {0.577f, 0.577f, 0.577f},
                 .sun_color = {1.0f, 1.0f, 1.0f},
                 .ibl_rotation = rendering::Scene::Transform(
                         Eigen::AngleAxisf(M_PI, Eigen::Vector3f::UnitX()))},
                {.name = "Bright day with sun at +Z",
                 .ibl_intensity = 45000,
                 .sun_intensity = 45000,
                 .sun_dir = {0.577f, 0.577f, -0.577f}},
                {.name = "Less bright day with sun at +Y",
                 .ibl_intensity = 35000,
                 .sun_intensity = 50000,
                 .sun_dir = {0.577f, -0.577f, -0.577f}},
                {.name = "Less bright day with sun at -Y",
                 .ibl_intensity = 35000,
                 .sun_intensity = 50000,
                 .sun_dir = {0.577f, 0.577f, 0.577f},
                 .sun_color = {1.0f, 1.0f, 1.0f},
                 .ibl_rotation = rendering::Scene::Transform(
                         Eigen::AngleAxisf(M_PI, Eigen::Vector3f::UnitX()))},
                {.name = "Less bright day with sun at +Z",
                 .ibl_intensity = 35000,
                 .sun_intensity = 50000,
                 .sun_dir = {0.577f, 0.577f, -0.577f}},
                {.name = POINT_CLOUD_PROFILE_NAME,
                 .ibl_intensity = 60000,
                 .sun_intensity = 50000,
                 .sun_dir = {0.577f, -0.577f, -0.577f},
                 .sun_color = {1.0f, 1.0f, 1.0f},
                 .ibl_rotation = rendering::Scene::Transform::Identity(),
                 .ibl_enabled = true,
                 .use_default_ibl = true,
                 .sun_enabled = false}};

const std::map<std::string, const GuiSettingsModel::LitMaterial>
        GuiSettingsModel::prefab_materials_ = {
                {DEFAULT_MATERIAL_NAME, {}},
                {"Metal (rougher)",
                 {{1.0f, 1.0f, 1.0f}, 1.0f, 0.7f, 0.5f, 0.0f, 0.0f, 0.0f}},
                {"Metal (smoother)",
                 {{1.0f, 1.0f, 1.0f}, 1.0f, 0.2f, 0.5f, 0.0f, 0.0f, 0.0f}},
                {"Plastic",
                 {{1.0f, 1.0f, 1.0f}, 0.0f, 0.5f, 0.5f, 0.5f, 0.8f, 0.0f}},
                {"Glazed ceramic",
                 {{1.0f, 1.0f, 1.0f}, 0.0f, 0.5f, 0.5f, 1.0f, 0.2f, 0.0f}},
                {"Clay",
                 {{0.7725f, 0.7725f, 0.7725f},
                  0.0f,
                  1.0f,
                  0.35f,
                  0.0f,
                  0.0f,
                  0.0f}},
};

const GuiSettingsModel::LightingProfile&
GuiSettingsModel::GetDefaultLightingProfile() {
    return GuiSettingsModel::lighting_profiles_[0];
}

const GuiSettingsModel::LightingProfile&
GuiSettingsModel::GetDefaultPointCloudLightingProfile() {
    for (auto& lp : GuiSettingsModel::lighting_profiles_) {
        if (lp.name == POINT_CLOUD_PROFILE_NAME) {
            return lp;
        }
    }
    utility::LogWarning(
            "Internal Error: could not find default point cloud lighting "
            "profile");
    return GuiSettingsModel::GetDefaultLightingProfile();
}

const GuiSettingsModel::LitMaterial& GuiSettingsModel::GetDefaultLitMaterial() {
    auto m = GuiSettingsModel::prefab_materials_.find(DEFAULT_MATERIAL_NAME);
    if (m != GuiSettingsModel::prefab_materials_.end()) {
        return m->second;
    }
    utility::LogError("Internal Error: could not find default lit material");
    // Makes compiler happy, but won't get here: LogError() throws exception
    // (Can throw here because this is a development error, and will happen
    // early in execution)
    static LitMaterial errorMaterial;
    return errorMaterial;
}

GuiSettingsModel::GuiSettingsModel() {
    lighting_ = GetDefaultLightingProfile();
    current_materials_.lit = GetDefaultLitMaterial();
}

bool GuiSettingsModel::GetShowSkybox() const { return show_skybox_; }
void GuiSettingsModel::SetShowSkybox(bool show) {
    show_skybox_ = show;
    NotifyChanged();
}

bool GuiSettingsModel::GetShowAxes() const { return show_axes_; }
void GuiSettingsModel::SetShowAxes(bool show) {
    show_axes_ = show;
    NotifyChanged();
}

const Eigen::Vector3f& GuiSettingsModel::GetBackgroundColor() const {
    return bg_color_;
}
void GuiSettingsModel::SetBackgroundColor(const Eigen::Vector3f& color) {
    bg_color_ = color;
    NotifyChanged();
}

const GuiSettingsModel::LightingProfile& GuiSettingsModel::GetLighting() const {
    return lighting_;
}
void GuiSettingsModel::SetLightingProfile(const LightingProfile& profile) {
    lighting_ = profile;
    user_has_changed_lighting_profile_ = true;
    user_has_customized_lighting_ = false;
    NotifyChanged();
}
void GuiSettingsModel::SetCustomLighting(const LightingProfile& profile) {
    lighting_ = profile;
    user_has_customized_lighting_ = true;
    NotifyChanged();
}

GuiSettingsModel::MaterialType GuiSettingsModel::GetMaterialType() const {
    return current_type_;
}
void GuiSettingsModel::SetMaterialType(MaterialType type) {
    current_type_ = type;
    NotifyChanged(true);
}

const GuiSettingsModel::Materials& GuiSettingsModel::GetCurrentMaterials()
        const {
    return current_materials_;
}

GuiSettingsModel::Materials& GuiSettingsModel::GetCurrentMaterials() {
    return current_materials_;
}

void GuiSettingsModel::SetLitMaterial(const LitMaterial& material,
                                      const std::string& name) {
    auto color = current_materials_.lit.base_color;
    current_materials_.lit = material;
    current_materials_.lit_name = name;
    if (user_has_changed_color_) {
        current_materials_.lit.base_color = color;
    }
    NotifyChanged(true);
}

void GuiSettingsModel::SetCurrentMaterials(const Materials& materials,
                                           const std::string& name) {
    current_materials_ = materials;
    current_materials_.lit_name = name;
    NotifyChanged(true);
}

void GuiSettingsModel::SetCurrentMaterials(const std::string& name) {
    current_materials_.lit_name = name;
    NotifyChanged(true);
}

void GuiSettingsModel::SetMaterialsToDefault() {
    auto unlit_color = current_materials_.unlit.base_color;
    auto lit_color = current_materials_.lit.base_color;
    current_materials_ = Materials();
    current_materials_.lit_name = DEFAULT_MATERIAL_NAME;
    if (user_has_changed_color_) {
        current_materials_.unlit.base_color = unlit_color;
        current_materials_.lit.base_color = lit_color;
    }
    NotifyChanged(true);
}

const Eigen::Vector3f& GuiSettingsModel::GetCurrentMaterialColor() const {
    if (user_has_changed_color_) {
        return current_materials_.lit.base_color;
    } else {
        if (current_type_ == LIT) {
            return current_materials_.lit.base_color;
        } else if (current_type_ == UNLIT) {
            return current_materials_.unlit.base_color;
        } else {
            static Eigen::Vector3f white = {1.0f, 1.0f, 1.0f};
            return white;
        }
    }
}

void GuiSettingsModel::SetCurrentMaterialColor(const Eigen::Vector3f& color) {
    if (current_type_ == LIT) {
        current_materials_.lit.base_color = color;
        user_has_changed_color_ = true;
        NotifyChanged(true);
    } else if (current_type_ == UNLIT) {
        current_materials_.unlit.base_color = color;
        user_has_changed_color_ = true;
        NotifyChanged(true);
    } else {
        // Doesn't make sense to set material color for depth / normal
    }
}

void GuiSettingsModel::ResetColors() {
    if (custom_default_color.x() >= 0.0f) {
        current_materials_.unlit.base_color = custom_default_color;
        current_materials_.lit.base_color = custom_default_color;
    } else {
        Materials defaults;
        current_materials_.unlit.base_color = defaults.unlit.base_color;
        current_materials_.lit.base_color = defaults.lit.base_color;
    }
    user_has_changed_color_ = false;
    NotifyChanged(true);
}

void GuiSettingsModel::SetCustomDefaultColor(const Eigen::Vector3f color) {
    custom_default_color = color;
}

void GuiSettingsModel::UnsetCustomDefaultColor() {
    custom_default_color = {-1.0f, -1.0f, -1.0f};
}

int GuiSettingsModel::GetPointSize() const {
    return current_materials_.point_size;
}

void GuiSettingsModel::SetPointSize(int size) {
    current_materials_.point_size = size;
    NotifyChanged(true);
}

bool GuiSettingsModel::GetDisplayingPointClouds() const {
    return displaying_point_clouds_;
}
void GuiSettingsModel::SetDisplayingPointClouds(bool displaying) {
    displaying_point_clouds_ = displaying;
    NotifyChanged();
}

bool GuiSettingsModel::GetUserHasChangedLightingProfile() const {
    return user_has_changed_lighting_profile_;
}

bool GuiSettingsModel::GetUserHasCustomizedLighting() const {
    return user_has_customized_lighting_;
}

bool GuiSettingsModel::GetUserHasChangedColor() const {
    return user_has_changed_color_;
}

void GuiSettingsModel::SetOnChanged(std::function<void(bool)> on_changed) {
    on_changed_ = on_changed;
}

void GuiSettingsModel::NotifyChanged(bool material_changed /*= false*/) {
    if (on_changed_) {
        on_changed_(material_changed);
    }
}

}  // namespace visualization
}  // namespace open3d
