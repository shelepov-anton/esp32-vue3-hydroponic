<template>
  <button class="button" :class="{primary: type === 'primary', secondary: type === 'secondary'}">
    <slot />
  </button>
</template>
<script lang="ts" setup>
const props = defineProps<{
  type: 'primary' | 'secondary'
}>()

</script>
<style lang="scss" scoped>
@import "@/assets/styles/main.scss";

.button {
  padding: 10px;
  border-radius: 36px;
  font-weight: bold;
  font-size: 22px;
  width: 100%;
  border: none;
}

.primary {
  background-color: $secondary;
  color: white;
}

.secondary {
  background-color: white;
  color: $primary;
}
</style>