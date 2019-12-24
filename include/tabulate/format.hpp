#pragma once
#include <algorithm>
#include <cstddef>
#include <optional>
#include <sstream>
#include <string>
#include <tabulate/color.hpp>
#include <tabulate/font_align.hpp>
#include <tabulate/font_style.hpp>
#include <vector>

namespace tabulate {

class Format {
public:
  Format &width(size_t value) {
    width_ = value;
    return *this;
  }

  Format &height(size_t value) {
    height_ = value;
    return *this;
  }

  Format &margin(size_t value) {
    margin_left_ = value;
    margin_right_ = value;
    margin_top_ = value;
    margin_bottom_ = value;
    return *this;
  }

  Format &margin_left(size_t value) {
    margin_left_ = value;
    return *this;
  }

  Format &margin_right(size_t value) {
    margin_right_ = value;
    return *this;
  }

  Format &margin_top(size_t value) {
    margin_top_ = value;
    return *this;
  }

  Format &margin_bottom(size_t value) {
    margin_bottom_ = value;
    return *this;
  }

  Format &padding(size_t value) {
    padding_left_ = value;
    padding_right_ = value;
    padding_top_ = value;
    padding_bottom_ = value;
    return *this;
  }

  Format &padding_left(size_t value) {
    padding_left_ = value;
    return *this;
  }

  Format &padding_right(size_t value) {
    padding_right_ = value;
    return *this;
  }

  Format &padding_top(size_t value) {
    padding_top_ = value;
    return *this;
  }

  Format &padding_bottom(size_t value) {
    padding_bottom_ = value;
    return *this;
  }

  Format &border(const std::string &value) {
    border_left_ = value;
    border_right_ = value;
    border_top_ = value;
    border_bottom_ = value;
    return *this;
  }

  Format &border_color(Color value) {
    border_left_color_ = value;
    border_right_color_ = value;
    border_top_color_ = value;
    border_bottom_color_ = value;
    return *this;
  }

  Format &border_background_color(Color value) {
    border_left_background_color_ = value;
    border_right_background_color_ = value;
    border_top_background_color_ = value;
    border_bottom_background_color_ = value;
    return *this;
  }

  Format &border_left(const std::string &value) {
    border_left_ = value;
    return *this;
  }

  Format &border_left_color(Color value) {
    border_left_color_ = value;
    return *this;
  }

  Format &border_left_background_color(Color value) {
    border_left_background_color_ = value;
    return *this;
  }

  Format &border_right(const std::string &value) {
    border_right_ = value;
    return *this;
  }

  Format &border_right_color(Color value) {
    border_right_color_ = value;
    return *this;
  }

  Format &border_right_background_color(Color value) {
    border_right_background_color_ = value;
    return *this;
  }

  Format &border_top(const std::string &value) {
    border_top_ = value;
    return *this;
  }

  Format &border_top_color(Color value) {
    border_top_color_ = value;
    return *this;
  }

  Format &border_top_background_color(Color value) {
    border_top_background_color_ = value;
    return *this;
  }

  Format &border_bottom(const std::string &value) {
    border_bottom_ = value;
    return *this;
  }

  Format &border_bottom_color(Color value) {
    border_bottom_color_ = value;
    return *this;
  }

  Format &border_bottom_background_color(Color value) {
    border_bottom_background_color_ = value;
    return *this;
  }

  Format &corner(const std::string &value) {
    corner_ = value;
    return *this;
  }

  Format &corner_color(Color value) {
    corner_color_ = value;
    return *this;
  }

  Format &corner_background_color(Color value) {
    corner_background_color_ = value;
    return *this;
  }

  Format &column_separator(const std::string &value) {
    column_separator_ = value;
    return *this;
  }

  Format &column_separator_color(Color value) {
    column_separator_color_ = value;
    return *this;
  }

  Format &column_separator_background_color(Color value) {
    column_separator_background_color_ = value;
    return *this;
  }

  Format &font_align(FontAlign value) {
    font_align_ = value;
    return *this;
  }

  Format &font_style(const std::vector<FontStyle> &style) {
    if (font_style_.has_value()) {
      for (auto &s : style)
        font_style_.value().push_back(s);
    } else {
      font_style_ = style;
    }
    return *this;
  }

  Format &font_color(Color value) {
    font_color_ = value;
    return *this;
  }

  Format &font_background_color(Color value) {
    font_background_color_ = value;
    return *this;
  }

  Format &color(Color value) {
    font_color(value);
    border_color(value);
    corner_color(value);
    return *this;
  }

  Format &background_color(Color value) {
    font_background_color(value);
    border_background_color(value);
    corner_background_color(value);
    return *this;
  }

  // Apply word wrap
  // Given an input string and a line length, this will insert \n
  // in strategic places in input string and apply word wrapping
  static std::string word_wrap(std::string str, size_t width) {
    std::vector<std::string> words = explode_string(str, {" ", "-", "\t"});
    size_t current_line_length = 0;
    std::string result;

    for (size_t i = 0; i < words.size(); ++i) {
      std::string word = words[i];
      // If adding the new word to the current line would be too long,
      // then put it on a new line (and split it up if it's too long).
      if (current_line_length + word.size() > width) {
        // Only move down to a new line if we have text on the current line.
        // Avoids situation where wrapped whitespace causes emptylines in text.
        if (current_line_length > 0) {
          result += '\n';
          current_line_length = 0;
        }

        // If the current word is too long to fit on a line even on it's own then
        // split the word up.
        while (word.size() > width) {
          result += word.substr(0, width - 1) + "-";
          word = word.substr(width - 1);
          result += '\n';
        }

        // Remove leading whitespace from the word so the new line starts flush to the left.
        word = trim_left(word);
      }
      result += word;
      current_line_length += word.size();
    }
    return result;
  }

  static std::vector<std::string> split_lines(const std::string &text,
                                              const std::string &delimiter) {
    std::vector<std::string> result{};
    std::string input = text;
    size_t pos = 0;
    std::string token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
      token = input.substr(0, pos);
      result.push_back(token);
      input.erase(0, pos + delimiter.length());
    }
    if (input.size())
      result.push_back(input);
    return result;
  };

  // Merge two formats
  // first has higher precedence
  // e.g., first = cell-level formatting and
  // second = row-level formatting
  // Result has attributes of both with cell-level
  // formatting taking precedence
  static Format merge(Format first, Format second) {
    Format result;

    // Width and height
    if (first.width_.has_value())
      result.width_ = first.width_;
    else
      result.width_ = second.width_;

    if (first.height_.has_value())
      result.height_ = first.height_;
    else
      result.height_ = second.height_;

    // Font styling
    if (first.font_align_.has_value())
      result.font_align_ = first.font_align_;
    else
      result.font_align_ = second.font_align_;

    if (first.font_style_.has_value()) {
      // Merge font styles using std::set_union
      std::vector<FontStyle> merged_font_style(first.font_style_.value().size() +
                                               second.font_style_.value().size());
      std::set_union(first.font_style_.value().begin(), first.font_style_.value().end(),
                     second.font_style_.value().begin(), second.font_style_.value().end(),
                     merged_font_style.begin());
      result.font_style_ = merged_font_style;
    } else
      result.font_style_ = second.font_style_;

    if (first.font_color_.has_value())
      result.font_color_ = first.font_color_;
    else
      result.font_color_ = second.font_color_;

    if (first.font_background_color_.has_value())
      result.font_background_color_ = first.font_background_color_;
    else
      result.font_background_color_ = second.font_background_color_;

    // Margin styling
    if (first.margin_left_.has_value())
      result.margin_left_ = first.margin_left_;
    else
      result.margin_left_ = second.margin_left_;

    if (first.margin_top_.has_value())
      result.margin_top_ = first.margin_top_;
    else
      result.margin_top_ = second.margin_top_;

    if (first.margin_right_.has_value())
      result.margin_right_ = first.margin_right_;
    else
      result.margin_right_ = second.margin_right_;

    if (first.margin_bottom_.has_value())
      result.margin_bottom_ = first.margin_bottom_;
    else
      result.margin_bottom_ = second.margin_bottom_;

    // Padding
    if (first.padding_left_.has_value())
      result.padding_left_ = first.padding_left_;
    else
      result.padding_left_ = second.padding_left_;

    if (first.padding_top_.has_value())
      result.padding_top_ = first.padding_top_;
    else
      result.padding_top_ = second.padding_top_;

    if (first.padding_right_.has_value())
      result.padding_right_ = first.padding_right_;
    else
      result.padding_right_ = second.padding_right_;

    if (first.padding_bottom_.has_value())
      result.padding_bottom_ = first.padding_bottom_;
    else
      result.padding_bottom_ = second.padding_bottom_;

    // Border
    if (first.border_left_.has_value())
      result.border_left_ = first.border_left_;
    else
      result.border_left_ = second.border_left_;

    if (first.border_left_color_.has_value())
      result.border_left_color_ = first.border_left_color_;
    else
      result.border_left_color_ = second.border_left_color_;

    if (first.border_left_background_color_.has_value())
      result.border_left_background_color_ = first.border_left_background_color_;
    else
      result.border_left_background_color_ = second.border_left_background_color_;

    if (first.border_top_.has_value())
      result.border_top_ = first.border_top_;
    else
      result.border_top_ = second.border_top_;

    if (first.border_top_color_.has_value())
      result.border_top_color_ = first.border_top_color_;
    else
      result.border_top_color_ = second.border_top_color_;

    if (first.border_top_background_color_.has_value())
      result.border_top_background_color_ = first.border_top_background_color_;
    else
      result.border_top_background_color_ = second.border_top_background_color_;

    if (first.border_bottom_.has_value())
      result.border_bottom_ = first.border_bottom_;
    else
      result.border_bottom_ = second.border_bottom_;

    if (first.border_bottom_color_.has_value())
      result.border_bottom_color_ = first.border_bottom_color_;
    else
      result.border_bottom_color_ = second.border_bottom_color_;

    if (first.border_bottom_background_color_.has_value())
      result.border_bottom_background_color_ = first.border_bottom_background_color_;
    else
      result.border_bottom_background_color_ = second.border_bottom_background_color_;

    if (first.border_right_.has_value())
      result.border_right_ = first.border_right_;
    else
      result.border_right_ = second.border_right_;

    if (first.border_right_color_.has_value())
      result.border_right_color_ = first.border_right_color_;
    else
      result.border_right_color_ = second.border_right_color_;

    if (first.border_right_background_color_.has_value())
      result.border_right_background_color_ = first.border_right_background_color_;
    else
      result.border_right_background_color_ = second.border_right_background_color_;

    // Corner
    if (first.corner_.has_value())
      result.corner_ = first.corner_;
    else
      result.corner_ = second.corner_;

    if (first.corner_color_.has_value())
      result.corner_color_ = first.corner_color_;
    else
      result.corner_color_ = second.corner_color_;

    if (first.corner_background_color_.has_value())
      result.corner_background_color_ = first.corner_background_color_;
    else
      result.corner_background_color_ = second.corner_background_color_;

    // Column separator
    if (first.column_separator_.has_value())
      result.column_separator_ = first.column_separator_;
    else
      result.column_separator_ = second.column_separator_;

    if (first.column_separator_color_.has_value())
      result.column_separator_color_ = first.column_separator_color_;
    else
      result.column_separator_color_ = second.column_separator_color_;

    if (first.column_separator_background_color_.has_value())
      result.column_separator_background_color_ = first.column_separator_background_color_;
    else
      result.column_separator_background_color_ = second.column_separator_background_color_;

    return result;
  }

private:
  friend class Cell;
  friend class Row;
  friend class Column;
  friend class TableInternal;
  friend class Printer;

  void set_defaults() {
    // NOTE: width and height are not set here
    font_align_ = FontAlign::left;
    font_style_ = std::vector<FontStyle>{};
    font_color_ = font_background_color_ = Color::none;
    margin_left_ = margin_right_ = margin_top_ = margin_bottom_ = 0;
    padding_left_ = padding_right_ = 1;
    padding_top_ = padding_bottom_ = 0;
    border_top_ = border_bottom_ = "-";
    border_left_ = border_right_ = "|";
    border_top_color_ = border_top_background_color_ = border_bottom_color_ =
        border_bottom_background_color_ = border_left_color_ = border_left_background_color_ =
            border_right_color_ = border_right_background_color_ = Color::none;
    corner_ = "+";
    corner_color_ = corner_background_color_ = Color::none;
    column_separator_ = "|";
    column_separator_color_ = column_separator_background_color_ = Color::none;
  }

  // Helper methods for word wrapping:

  // trim white spaces from the left end of an input string
  static std::string trim_left(const std::string &input_string) {
    std::string result = input_string;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(),
                                              [](int ch) { return !std::isspace(ch); }));
    return result;
  }

  // trim white spaces from right end of an input string
  static std::string trim_right(const std::string &input_string) {
    std::string result = input_string;
    result.erase(
        std::find_if(result.rbegin(), result.rend(), [](int ch) { return !std::isspace(ch); }).base(),
        result.end());
    return result;
  }

  // trim white spaces from either end of an input string
  static std::string trim(const std::string &input_string) { return trim_left(trim_right(input_string)); }

  static size_t index_of_any(const std::string &input, size_t start_index,
                             const std::vector<std::string> &split_characters) {
    std::vector<size_t> indices{};
    for (auto &c : split_characters) {
      auto index = input.find(c, start_index);
      if (index != std::string::npos)
        indices.push_back(index);
    }
    if (indices.size() > 0)
      return *std::min_element(indices.begin(), indices.end());
    else
      return std::string::npos;
  }

  static std::vector<std::string> explode_string(const std::string &input,
                                                 const std::vector<std::string> &split_characters) {
    std::vector<std::string> result{};
    size_t start_index{0};
    while (true) {
      auto index = index_of_any(input, start_index, split_characters);

      if (index == std::string::npos) {
        result.push_back(input.substr(start_index));
        return result;
      }

      std::string word = input.substr(start_index, index - start_index);
      char next_character = input.substr(index, 1)[0];
      // Unlike whitespace, dashes and the like should stick to the word occurring before it.
      if (isspace(next_character)) {
        result.push_back(word);
        result.push_back(std::string(1, next_character));
      } else {
        result.push_back(word + next_character);
      }
      start_index = index + 1;
    }

    return result;
  }

  // Element width and height
  std::optional<size_t> width_{};
  std::optional<size_t> height_{};

  // Font styling
  std::optional<FontAlign> font_align_{};
  std::optional<std::vector<FontStyle>> font_style_{};
  std::optional<Color> font_color_{};
  std::optional<Color> font_background_color_{};

  // Margin styling
  std::optional<size_t> margin_left_{};
  std::optional<size_t> margin_top_{};
  std::optional<size_t> margin_right_{};
  std::optional<size_t> margin_bottom_{};

  // Element padding
  std::optional<size_t> padding_left_{};
  std::optional<size_t> padding_top_{};
  std::optional<size_t> padding_right_{};
  std::optional<size_t> padding_bottom_{};

  // Element border
  std::optional<std::string> border_top_{};
  std::optional<Color> border_top_color_{};
  std::optional<Color> border_top_background_color_{};

  std::optional<std::string> border_bottom_{};
  std::optional<Color> border_bottom_color_{};
  std::optional<Color> border_bottom_background_color_{};

  std::optional<std::string> border_left_{};
  std::optional<Color> border_left_color_{};
  std::optional<Color> border_left_background_color_{};

  std::optional<std::string> border_right_{};
  std::optional<Color> border_right_color_{};
  std::optional<Color> border_right_background_color_{};

  // Element corner
  std::optional<std::string> corner_{};
  std::optional<Color> corner_color_{};
  std::optional<Color> corner_background_color_{};

  // Element column separator
  std::optional<std::string> column_separator_{};
  std::optional<Color> column_separator_color_{};
  std::optional<Color> column_separator_background_color_{};
};

}; // namespace tabulate