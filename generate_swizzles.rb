#!ruby

vec2 = %w[x y]
vec3 = %w[x y z]
vec4 = %w[x y z w]

vec2_pairs = vec2.product(vec2)
vec3_pairs = vec3.product(vec3)
vec3_triples = vec3.product(vec3, vec3)
vec4_pairs = vec4.product(vec4)
vec4_triples = vec4.product(vec4, vec4)
vec4_quads = vec4.product(vec4, vec4, vec4)

def generate_swizzles_and_splats(filename, vecN, combos2, combos3 = [], combos4 = [])
  lines = []

  combos2.each do |a, b|
    lines << ([a, b].uniq.size == 2 ? "swizzle_#{vecN}_v2(#{a},#{b})" : "splat_#{vecN}_v2(#{a},#{b})")
  end

  combos3.each do |a, b, c|
    lines << ([a, b, c].uniq.size == 3 ? "swizzle_#{vecN}_v3(#{a},#{b},#{c})" : "splat_#{vecN}_v3(#{a},#{b},#{c})")
  end

  combos4.each do |a, b, c, d|
    lines << ([a, b, c, d].uniq.size == 4 ? "swizzle_#{vecN}_v4(#{a},#{b},#{c},#{d})" : "splat_#{vecN}_v4(#{a},#{b},#{c},#{d})")
  end

  File.open(filename, "w") do |file|
    file.puts lines.sort
  end
end

generate_swizzles_and_splats("vec2_swizzles.inl", "v2", vec2_pairs)
generate_swizzles_and_splats("vec3_swizzles.inl", "v3", vec3_pairs, vec3_triples)
generate_swizzles_and_splats("vec4_swizzles.inl", "v4", vec4_pairs, vec4_triples, vec4_quads)