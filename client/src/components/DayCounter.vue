<template>
  <div class="container">
    <h1 class="text-4xl">{{ count }}</h1>
    <p class="text-lg">дней до созревания</p>
  </div>
</template>

<script lang="ts" setup>
const props = defineProps<{
  count: number
}>()
</script>

<style lang="scss" scoped>
.container {
  display: flex;
  flex-direction: column;
  align-items: center;
}
</style>