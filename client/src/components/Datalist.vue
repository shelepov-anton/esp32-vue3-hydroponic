<template>
  <div class="grid">
    <template v-for="item in data">
      <span>{{ item.title }}</span>
      <span>{{ item.value }}</span>
    </template>
  </div>
</template>
<script lang="ts" setup>
import { computed } from 'vue'

const props = defineProps<{
  phValue: number
}>()

const data = computed(() => [
  { title: 'Уровень pH', value: props.phValue },
  { title: 'Температура', value: '25º' },
  { title: 'Уровень воды', value: '200 мл' }
])

</script>
<style lang="scss" scoped>
.grid {
  display: grid;
  grid-template-columns: 3fr 1fr;
  width: 100%;
  row-gap: 22px;
}
</style>